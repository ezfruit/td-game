#include "raylib.h"
#include "raymath.h"
#include "enemy.h"
#include "sounds.h"
#include "play.h"
#include <memory>

Enemy::Enemy(int health, float speed) : health(health), speed(speed), baseSpeed(speed) {}

Sound metal;

namespace SoundManager {
    extern Sound shootSound;

    void InitSounds();
    void UnloadSounds();
}

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

void Enemy::update(float deltaTime, const std::vector<Vector2>& track) {
    if (currentTarget >= track.size()) return;

    if (deltaTime > 0.05f) deltaTime = 0.05f;

    float currentTime = GetTime();

    if (burning) {
        if (currentTime >= burnEndTime) {
            burning = false;
            revertSpeed();
        } else if (currentTime >= nextBurnTick) {
            takeDamage(burnDamage, "Fire");
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
        
bool Enemy::isAlive() const { 
    return alive; 
}

bool Enemy::isBurning() const {
    return burning;
}

void Enemy::setCurrentTarget(int target) {
    currentTarget = target;
}

void Enemy::applyBurn(float delay, float dps, float duration, float slowEffect) {
    burning = true;
    speed *= slowEffect;
    burnDuration = duration;
    burnDamage = dps;
    burnDelay = delay;
    burnStartTime = GetTime();
    burnEndTime = burnStartTime + duration;
    nextBurnTick = burnStartTime + delay;
}

Slime::Slime() : Enemy(4, 50.0f) { // 50 speed for normal
    maxHealth = health;
} 

std::string Slime::getName() const {
    return "Slime";
}

void Slime::takeDamage(int amount, const std::string& type) {
    health -= amount;
    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

Knight::Knight() : Enemy(10, 40.0f) { // 40 speed for normal
    maxHealth = health;
}

std::string Knight::getName() const {
    return "Knight";
}

void Knight::takeDamage(int amount, const std::string& type) {
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

Fire_Imp::Fire_Imp() : Enemy(25, 100.0f) { // 100 speed for normal
    maxHealth = health;
}

std::string Fire_Imp::getName() const {
    return "Fire Imp";
}

void Fire_Imp::takeDamage(int amount, const std::string& type) {
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

Brute::Brute() : Enemy(50, 25.0f) { // 25 speed for normal
    maxHealth = health;
}

std::string Brute::getName() const {
    return "Brute";
}

void Brute::takeDamage(int amount, const std::string& type) {
    health -= amount;
    if (health <= 0) {
        alive = false;
        health = 0;
    }
}


SpawnableEnemy::SpawnableEnemy(float hp, float speed, float delay, float cooldown, int spawnCount) : Enemy(hp, speed), spawnDelay(delay), spawnCooldown(cooldown), spawnCount(spawnCount) {
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

Spider_Queen::Spider_Queen() : SpawnableEnemy(300, 25.0f, 1.0f, 5.0f, 3) { // 25 speed for normal
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

void Spider_Queen::takeDamage(int amount, const std::string& type) {
    health -= amount;
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

Spiderling::Spiderling() : Enemy(10, 80.0f) {
    maxHealth = health;
}

std::string Spiderling::getName() const {
    return "Spiderling";
}

void Spiderling::takeDamage(int amount, const std::string& type) {
    health -= amount;
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
