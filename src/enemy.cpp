#include "raylib.h"
#include "raymath.h"
#include "enemy.h"
#include "sounds.h"
#include "play.h"
#include <memory>
#include "tower.h"

Enemy::Enemy(int health, float speed, float radius) : health(health), speed(speed), baseSpeed(speed), hitboxRadius(radius) {}

std::vector<std::shared_ptr<Enemy>> deathSpawns;

void Enemy::reduceSpeed(float spd) {
    speed -= spd;
    if (speed < 0) {
        speed = 0;
    }
}

void Enemy::revertSpeed() {
    speed = baseSpeed;
}

void Enemy::setPosition(Vector2 pos) {
    position = pos;
}

float Enemy::getPathProgress(const std::vector<Vector2>& track) const {
    if (currentTarget == 0 || currentTarget >= track.size())
        return static_cast<float>(currentTarget);

    float segmentLength = Vector2Distance(track[currentTarget - 1], track[currentTarget]);
    float segmentProgress = Vector2Distance(position, track[currentTarget]);

    return static_cast<float>(currentTarget - 1) + (1.0f - (segmentProgress / segmentLength));
}

void Enemy::update(float deltaTime, const std::vector<Vector2>& track) {
    if (currentTarget >= track.size()) return;

    if (deltaTime > 0.05f) deltaTime = 0.05f;

    float currentTime = GetTime();

    if (burning) {
        if (currentTime >= burnEndTime) {
            burning = false;
            revertSpeed();
        } else if (currentTime >= nextBurnTick) {
            int prevHealth = getHealth();
            takeDamage(burnDamage, "Fire", "Single");
            int curHealth = getHealth();
            int damageDealt = prevHealth - curHealth;
            if (auto tower = burnSource.lock()) {
                tower->setTotalDamageDealt(damageDealt);
            }
            playerMoney += damageDealt;
            nextBurnTick += burnDelay;
        }
    }

    Vector2 target = track[currentTarget];
    Vector2 direction = Vector2Normalize(Vector2Subtract(target, position));
    position = Vector2Add(position, Vector2Scale(direction, speed * deltaTime));

    if (Vector2Distance(position, target) < 1.0f) {
        currentTarget++;
    }
}

Vector2 Enemy::getPosition() const {
    return position;
}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getMaxHealth() const {
    return maxHealth;
}

float Enemy::getRadius() const {
    return hitboxRadius;
}
        
bool Enemy::isAlive() const { 
    return alive; 
}

bool Enemy::isBurning() const {
    return burning;
}

void Enemy::setCurrentTarget(int target) {
    currentTarget = target;
}

void Enemy::applyBurn(float delay, float dps, float duration, float slowEffect, std::weak_ptr<Tower> source) {
    burning = true;
    speed *= slowEffect;
    burnDuration = duration;
    burnDamage = dps;
    burnDelay = delay;
    burnStartTime = GetTime();
    burnEndTime = burnStartTime + duration;
    nextBurnTick = burnStartTime + delay;
    burnSource = source;
}

Slime::Slime() : Enemy(4, 50.0f, 10.0f) { // 50 speed for normal
    maxHealth = health;
} 

std::string Slime::getName() const {
    return "Slime";
}

void Slime::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    health -= amount;
    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Slime::draw() const {
    DrawCircleV(position, hitboxRadius, GREEN);
}

Knight::Knight() : Enemy(10, 40.0f, 10.0f) { // 40 speed for normal
    maxHealth = health;
}

std::string Knight::getName() const {
    return "Knight";
}

void Knight::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (type == "Magic") {
        health -= static_cast<int> (amount * 1.5);
    } else if (type == "Physical") {
        health -= static_cast<int> (amount * 0.5);
    } else {
        health -= amount;
    }
    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Knight::draw() const {
    DrawCircleV(position, hitboxRadius, GRAY);
}

Fire_Imp::Fire_Imp() : Enemy(25, 100.0f, 10.0f) { // 100 speed for normal
    maxHealth = health;
}

std::string Fire_Imp::getName() const {
    return "Fire Imp";
}

void Fire_Imp::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (type == "Fire") {
        burning = false;
        revertSpeed();
        return;
    } else {
        health -= amount;
    }
    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Fire_Imp::draw() const {
    DrawCircleV(position, hitboxRadius, ORANGE);
}


Brute::Brute() : Enemy(50, 25.0f, 30.0f) { // 25 speed for normal
    maxHealth = health;
}

std::string Brute::getName() const {
    return "Brute";
}

void Brute::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    health -= amount;
    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Brute::draw() const {
    DrawCircleV(position, hitboxRadius, BROWN);
}

SpawnableEnemy::SpawnableEnemy(float hp, float speed, float radius, float delay, float cooldown, int spawnCount) : Enemy(hp, speed, radius), spawnDelay(delay), spawnCooldown(cooldown), spawnCount(spawnCount) {
    remainingSpawns = spawnCount;
}

void SpawnableEnemy::update(float deltaTime, const std::vector<Vector2>& track) {
    Enemy::update(deltaTime, track);

    if (inCooldown) {
        spawnCooldownTimer += deltaTime;
        if (spawnCooldownTimer >= spawnCooldown) {
            inCooldown = false;
            remainingSpawns = spawnCount;  // reset for new wave
            spawnDelayTimer = 0.0f;
        }
    } else {
        spawnDelayTimer += deltaTime;
        if (remainingSpawns > 0 && spawnDelayTimer >= spawnDelay) {
            spawn(); // spawn enemy
            remainingSpawns--;
            spawnDelayTimer = 0.0f;
        }

        if (remainingSpawns == 0) { // once this spawn is over, go on cooldown
            inCooldown = true;
            spawnCooldownTimer = 0.0f;
        }
    }
}

Spider_Queen::Spider_Queen() : SpawnableEnemy(300, 25.0f, 20.0f, 1.0f, 5.0f, 3) { // 25 speed for normal
    maxHealth = health;
}

void Spider_Queen::spawn() {
    Vector2 spawnPos = getPosition();
    std::shared_ptr<Enemy> enemy = std::make_shared<Spiderling>();
    enemy->setPosition(spawnPos);
    enemy->setCurrentTarget(currentTarget);
    enemies.push_back(std::move(enemy));
}

std::string Spider_Queen::getName() const {
    return "Spider Queen";
}

void Spider_Queen::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (type == "Fire") {
        health -= static_cast<int> (amount * 2);
    } else {
        health -= amount;
    }

    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Spider_Queen::draw() const {
    DrawCircleV(position, hitboxRadius, BLACK);
}

Spiderling::Spiderling() : Enemy(10, 80.0f, 5.0f) {
    maxHealth = health;
}

std::string Spiderling::getName() const {
    return "Spiderling";
}

void Spiderling::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (type == "Fire") {
        health -= static_cast<int> (amount * 2);
    } else {
        health -= amount;
    }

    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Spiderling::draw() const {
    DrawCircleV(position, hitboxRadius, BLACK);
}

Arcane_Shell::Arcane_Shell() : Enemy(100, 60.0f, 15.0f) {
    maxHealth = health;
}

std::string Arcane_Shell::getName() const {
    return "Arcane Shell";
}

void Arcane_Shell::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (type == "Magic") {
        return;
    } else if (type == "Physical") {
        health -= static_cast<int> (amount * 1.5);
    } else {
        health -= static_cast<int> (amount * 0.5);
    }

    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Arcane_Shell::draw() const {
    DrawCircleV(position, hitboxRadius, PINK);
}

Flux::Flux() : Enemy(250, 30.0f, 14.0f) {
    maxHealth = health;
}

void Flux::update(float deltaTime, const std::vector<Vector2>& track) {
    
    Enemy::update(deltaTime, track);

    shieldChangeTimer += GetFrameTime();

    if (shieldChangeTimer >= shieldChangeCooldown) {
        if (shield == "Physical") {
            shield = "Magic";
        } else {
            shield = "Physical";
        }
        shieldChangeTimer = 0.0f;
    }
}

std::string Flux::getName() const {
    return "Flux";
}

void Flux::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (shield == type) {
        speed += 1;
        return;
    } else {
        health -= amount;
    }

    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Flux::draw() const {
    Color ringColor = (shield == "Physical") ? GRAY : PINK;
    // Draw outer ring for the shield
    DrawCircleLinesV(position, hitboxRadius, ringColor);
    // Draw inner enemy body
    DrawCircleV(position, 12, YELLOW);
}

void Flux::setShield(std::string newShield) {
    shield = newShield;
}

std::string Flux::getShield() const {
    return shield;
}

Husk::Husk() : Enemy(75, 150.0f, 20.0f) {
    maxHealth = health;
}

std::string Husk::getName() const {
    return "Husk";
}

void Husk::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (type == "Air" || type == "Fire") {
        health -= static_cast<int> (amount * 3);
    } else {
        health -= amount;
    }

    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Husk::draw() const {
    DrawCircleV(position, hitboxRadius, DARKGRAY);
}

Exoskeleton::Exoskeleton() : Enemy(500, 35.0f, 12.0f) {
    maxHealth = health;
}

std::string Exoskeleton::getName() const {
    return "Exoskeleton";
}

void Exoskeleton::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    
    if (targeting == "Pierce") {
        health -= static_cast<int> (amount * 2);
    } else if (targeting == "Area of Effect") {
        health -= static_cast<int> (amount * 0.5);
    } else {
        health -= amount;
    }

    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Exoskeleton::draw() const {
    DrawCircleV(position, hitboxRadius, LIGHTGRAY);
}

Goliath::Goliath() : Enemy(1200, 20.0f, 20.0f) {
    maxHealth = health;
}

std::string Goliath::getName() const {
    return "Goliath";
}

void Goliath::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (type == "Air") {
        health -= static_cast<int> (amount * 0.5);
    } else {
        health -= amount;
    }

    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Goliath::draw() const {
    DrawCircleV(position, hitboxRadius, DARKPURPLE);
}

Sludge::Sludge() : Enemy(350, 60.0f, 15.0f) {
    maxHealth = health;
}

std::string Sludge::getName() const {
    return "Sludge";
}

void Sludge::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (type == "Fire") {
        health -= static_cast<int> (amount * 4);
    } else if (type == "Physical" || targeting == "Pierce") {
        return;
    } else {
        health -= amount;
    }

    if (health <= 0) {
        alive = false;
        health = 0;
        spawn();
    }
}

void Sludge::spawn() {
    Vector2 spawnPos = getPosition();
    std::shared_ptr<Enemy> enemy = std::make_shared<Sludge_Mite>();
    enemy->setPosition(spawnPos);
    enemy->setCurrentTarget(currentTarget);
    deathSpawns.push_back(std::move(enemy));
}

void Sludge::draw() const {
    DrawCircleV(position, hitboxRadius, DARKBROWN);
}

Sludge_Mite::Sludge_Mite() : Enemy(100, 40.0f, 10.0f) {
    maxHealth = health;
}

std::string Sludge_Mite::getName() const {
    return "Sludge Mite";
}

void Sludge_Mite::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (type == "Fire") {
        health -= static_cast<int> (amount * 4);
    } else if (type == "Physical" || targeting == "Pierce") {
        return;
    } else {
        health -= amount;
    }

    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Sludge_Mite::draw() const {
    DrawCircleV(position, hitboxRadius, DARKBROWN);
}

Lava_Golem::Lava_Golem() : Enemy(600, 30.0f, 25.0f) {
    maxHealth = health;
}

std::string Lava_Golem::getName() const {
    return "Lava Golem";
}

void Lava_Golem::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (type == "Fire" || type == "Air") {
        burning = false;
        revertSpeed();
        health -= static_cast<int> (amount * 0.5);
    } else {
        health -= amount;
    }
    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Lava_Golem::draw() const {
    DrawCircleV(position, hitboxRadius, ORANGE);
}

Obsidian_Behemoth::Obsidian_Behemoth() : Enemy(2500, 20.0f, 30.0f) {
    maxHealth = health;
}

std::string Obsidian_Behemoth::getName() const {
    return "Obsidian Behemoth";
}

void Obsidian_Behemoth::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    health -= amount;

    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Obsidian_Behemoth::draw() const {
    DrawCircleV(position, hitboxRadius, BLACK);
}