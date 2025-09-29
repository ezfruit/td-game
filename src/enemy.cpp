#include "raylib.h"
#include "raymath.h"
#include "enemy.h"
#include "sounds.h"
#include "play.h"
#include <memory>
#include "tower.h"

Enemy::Enemy(int health, float speed, float radius) : health(health), maxHealth(health), speed(speed), baseSpeed(speed), hitboxRadius(radius) {}

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

    animationTimer += deltaTime;

    if (!moveFrames.empty()) {
        if (animationTimer >= frameSpeed) {
            animationTimer = 0.0f;
            currentFrame = (currentFrame + 1) % moveFrames.size();
        }
    }

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

void Enemy::unloadFrames() {
    for (auto& frame : moveFrames) {
        UnloadTexture(frame);
    }
    moveFrames.clear();
}

Slime::Slime() : Enemy(4, 50.0f, 10.0f) {
    moveFrames = ImageHandler::LoadAnimationFrames("slime", 8);
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
    Texture2D frame = moveFrames[currentFrame];
    float diameter = hitboxRadius * 2.0f;

    Rectangle dest = {
        position.x,
        position.y,
        diameter,
        diameter
    };

    Vector2 origin = {
        diameter / 2.0f,
        diameter / 2.0f
    };

    float angleDeg = 0.0f;

    if (currentTarget < trackPoints.size()) {
        Vector2 dir = Vector2Subtract(trackPoints[currentTarget], position);
        float angleRad = atan2f(dir.y, dir.x);
        angleDeg = angleRad * (180.0f / PI);

        // Flip horizontally when moving left
        if (angleDeg > 90.0f || angleDeg < -90.0f) {
            angleDeg += 180.0f;
            // Flip source rect too
            Rectangle source = {
                0.0f, 0.0f,
                (float)-frame.width, // flip horizontally
                (float)frame.height
            };
            DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
            return;
        }
    }

    // Default source rect (not flipped)
    Rectangle source = {
        0.0f, 0.0f,
        (float)frame.width,
        (float)frame.height
    };

    DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
}

Knight::Knight() : Enemy(10, 40.0f, 10.0f) {}

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

Fire_Imp::Fire_Imp() : Enemy(25, 100.0f, 15.0f) {
    moveFrames = ImageHandler::LoadAnimationFrames("fire_imp", 4);
    frameSpeed = 0.1f;
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
    Texture2D frame = moveFrames[currentFrame];
    float diameter = hitboxRadius * 2.0f;

    Rectangle dest = {
        position.x,
        position.y,
        diameter,
        diameter
    };

    Vector2 origin = {
        diameter / 2.0f,
        diameter / 2.0f
    };

    float angleDeg = 0.0f;

    if (currentTarget < trackPoints.size()) {
        Vector2 dir = Vector2Subtract(trackPoints[currentTarget], position);
        float angleRad = atan2f(dir.y, dir.x);
        angleDeg = angleRad * (180.0f / PI);

        // Flip horizontally when moving left
        if (angleDeg > 90.0f || angleDeg < -90.0f) {
            angleDeg += 180.0f;
            // Flip source rect too
            Rectangle source = {
                0.0f, 0.0f,
                (float)-frame.width, // flip horizontally
                (float)frame.height
            };
            DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
            return;
        }
    }

    // Default source rect (not flipped)
    Rectangle source = {
        0.0f, 0.0f,
        (float)frame.width,
        (float)frame.height
    };

    DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
}


Brute::Brute() : Enemy(50, 25.0f, 30.0f) {}

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

Spider_Queen::Spider_Queen() : SpawnableEnemy(300, 25.0f, 20.0f, 1.0f, 8.0f, 3) {}

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

Spiderling::Spiderling() : Enemy(10, 80.0f, 5.0f) {}

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
    moveFrames = ImageHandler::LoadAnimationFrames("arcane_shell", 8);
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
    Texture2D frame = moveFrames[currentFrame];
    float diameter = hitboxRadius * 2.0f;

    Rectangle dest = {
        position.x,
        position.y,
        diameter,
        diameter
    };

    Vector2 origin = {
        diameter / 2.0f,
        diameter / 2.0f
    };

    float angleDeg = 0.0f;

    // Default source rect (not flipped)
    Rectangle source = {
        0.0f, 0.0f,
        (float)frame.width,
        (float)frame.height
    };

    DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
}

Flux::Flux() : Enemy(250, 30.0f, 14.0f) {
    moveFrames = ImageHandler::LoadAnimationFrames("flux", 4);
    frameSpeed = 0.25f;
}

void Flux::update(float deltaTime, const std::vector<Vector2>& track) {
    
    if (currentTarget >= track.size()) return;

    if (deltaTime > 0.05f) deltaTime = 0.05f;

    float currentTime = GetTime();

    animationTimer += deltaTime;

    if (!moveFrames.empty()) {
        if (animationTimer >= frameSpeed) {
            animationTimer = 0.0f;
            int currentFrameIndex = (currentFrame + 1) % 2;
            currentFrame = shieldFrameIndex + currentFrameIndex;
        }
    }

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
    
    shieldChangeTimer += GetFrameTime();

    if (shieldChangeTimer >= shieldChangeCooldown) {
        if (shield == "Physical") {
            shield = "Magic";
            shieldFrameIndex = 2;
        } else {
            shield = "Physical";
            shieldFrameIndex = 0;
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
    Texture2D frame = moveFrames[currentFrame];
    float diameter = hitboxRadius * 2.0f;

    Rectangle dest = {
        position.x,
        position.y,
        diameter,
        diameter
    };

    Vector2 origin = {
        diameter / 2.0f,
        diameter / 2.0f
    };

    float angleDeg = 0.0f;

    // Default source rect (not flipped)
    Rectangle source = {
        0.0f, 0.0f,
        (float)frame.width,
        (float)frame.height
    };

    DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
}

void Flux::setShield(std::string newShield) {
    shield = newShield;
}

std::string Flux::getShield() const {
    return shield;
}

Husk::Husk() : Enemy(75, 150.0f, 20.0f) {
    frameSpeed = 0.25f;
    moveFrames = ImageHandler::LoadAnimationFrames("husk", 4);
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
    Texture2D frame = moveFrames[currentFrame];
    float diameter = hitboxRadius * 2.0f;

    Rectangle dest = {
        position.x,
        position.y,
        diameter,
        diameter
    };

    Vector2 origin = {
        diameter / 2.0f,
        diameter / 2.0f
    };

    float angleDeg = 0.0f;

    if (currentTarget < trackPoints.size()) {
        Vector2 dir = Vector2Subtract(trackPoints[currentTarget], position);
        float angleRad = atan2f(dir.y, dir.x);
        angleDeg = angleRad * (180.0f / PI);

        // Flip horizontally when moving left
        if (angleDeg > 90.0f || angleDeg < -90.0f) {
            angleDeg += 180.0f;
            // Flip source rect too
            Rectangle source = {
                0.0f, 0.0f,
                (float)-frame.width, // flip horizontally
                (float)frame.height
            };
            DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
            return;
        }
    }

    // Default source rect (not flipped)
    Rectangle source = {
        0.0f, 0.0f,
        (float)frame.width,
        (float)frame.height
    };

    DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
}

Exoskeleton::Exoskeleton() : Enemy(500, 35.0f, 12.0f) {
    frameSpeed = 0.5f;
    moveFrames = ImageHandler::LoadAnimationFrames("exoskeleton", 2);
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
    Texture2D frame = moveFrames[currentFrame];
    float diameter = hitboxRadius * 2.0f;

    Rectangle dest = {
        position.x,
        position.y,
        diameter,
        diameter
    };

    Vector2 origin = {
        diameter / 2.0f,
        diameter / 2.0f
    };

    float angleDeg = 0.0f;

    if (currentTarget < trackPoints.size()) {
        Vector2 dir = Vector2Subtract(trackPoints[currentTarget], position);
        float angleRad = atan2f(dir.y, dir.x);
        angleDeg = angleRad * (180.0f / PI);

        // Flip horizontally when moving left
        if (angleDeg > 90.0f || angleDeg < -90.0f) {
            angleDeg += 180.0f;
            // Flip source rect too
            Rectangle source = {
                0.0f, 0.0f,
                (float)-frame.width, // flip horizontally
                (float)frame.height
            };
            DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
            return;
        }
    }

    // Default source rect (not flipped)
    Rectangle source = {
        0.0f, 0.0f,
        (float)frame.width,
        (float)frame.height
    };

    DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
}

Goliath::Goliath() : Enemy(1200, 20.0f, 20.0f) {}

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
    frameSpeed = 0.33f;
    moveFrames = ImageHandler::LoadAnimationFrames("sludge", 2);
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
    Texture2D frame = moveFrames[currentFrame];
    float diameter = hitboxRadius * 2.0f;

    Rectangle dest = {
        position.x,
        position.y,
        diameter,
        diameter
    };

    Vector2 origin = {
        diameter / 2.0f,
        diameter / 2.0f
    };

    float angleDeg = 0.0f;

    if (currentTarget < trackPoints.size()) {
        Vector2 dir = Vector2Subtract(trackPoints[currentTarget], position);
        float angleRad = atan2f(dir.y, dir.x);
        angleDeg = angleRad * (180.0f / PI);

        // Flip horizontally when moving left
        if (angleDeg > 90.0f || angleDeg < -90.0f) {
            angleDeg += 180.0f;
            // Flip source rect too
            Rectangle source = {
                0.0f, 0.0f,
                (float)-frame.width, // flip horizontally
                (float)frame.height
            };
            DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
            return;
        }
    }

    // Default source rect (not flipped)
    Rectangle source = {
        0.0f, 0.0f,
        (float)frame.width,
        (float)frame.height
    };

    DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
}

Sludge_Mite::Sludge_Mite() : Enemy(100, 40.0f, 10.0f) {
    frameSpeed = 0.33f;
    moveFrames = ImageHandler::LoadAnimationFrames("sludge_mite", 2);
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
    Texture2D frame = moveFrames[currentFrame];
    float diameter = hitboxRadius * 2.0f;

    Rectangle dest = {
        position.x,
        position.y,
        diameter,
        diameter
    };

    Vector2 origin = {
        diameter / 2.0f,
        diameter / 2.0f
    };

    float angleDeg = 0.0f;

    if (currentTarget < trackPoints.size()) {
        Vector2 dir = Vector2Subtract(trackPoints[currentTarget], position);
        float angleRad = atan2f(dir.y, dir.x);
        angleDeg = angleRad * (180.0f / PI);

        // Flip horizontally when moving left
        if (angleDeg > 90.0f || angleDeg < -90.0f) {
            angleDeg += 180.0f;
            // Flip source rect too
            Rectangle source = {
                0.0f, 0.0f,
                (float)-frame.width, // flip horizontally
                (float)frame.height
            };
            DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
            return;
        }
    }

    // Default source rect (not flipped)
    Rectangle source = {
        0.0f, 0.0f,
        (float)frame.width,
        (float)frame.height
    };

    DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
}

Lava_Golem::Lava_Golem() : Enemy(600, 30.0f, 25.0f) {
    frameSpeed = 0.25f;
    moveFrames = ImageHandler::LoadAnimationFrames("lava_golem", 4);
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
    Texture2D frame = moveFrames[currentFrame];
    float diameter = hitboxRadius * 2.0f;

    Rectangle dest = {
        position.x,
        position.y,
        diameter,
        diameter
    };

    Vector2 origin = {
        diameter / 2.0f,
        diameter / 2.0f
    };

    float angleDeg = 0.0f;

    if (currentTarget < trackPoints.size()) {
        Vector2 dir = Vector2Subtract(trackPoints[currentTarget], position);
        float angleRad = atan2f(dir.y, dir.x);
        angleDeg = angleRad * (180.0f / PI);

        // Flip horizontally when moving left
        if (angleDeg > 90.0f || angleDeg < -90.0f) {
            angleDeg += 180.0f;
            // Flip source rect too
            Rectangle source = {
                0.0f, 0.0f,
                (float)-frame.width, // flip horizontally
                (float)frame.height
            };
            DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
            return;
        }
    }

    // Default source rect (not flipped)
    Rectangle source = {
        0.0f, 0.0f,
        (float)frame.width,
        (float)frame.height
    };

    DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
}

Obsidian_Behemoth::Obsidian_Behemoth() : Enemy(2500, 20.0f, 30.0f) {}

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

Ravager::Ravager() : Enemy(800, 75.0f, 35.0f) {}

std::string Ravager::getName() const {
    return "Ravager";
}

void Ravager::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (type == "Magic") {
        health -= static_cast<int> (amount * 2);
    } else {
        health -= amount;
    }

    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Ravager::draw() const {
    DrawCircleV(position, hitboxRadius, DARKBLUE);
}

Arcane_Warden::Arcane_Warden() : SpawnableEnemy(1500, 30.0f, 25.0f, 0.5f, 6.0f, 4) {
    frameSpeed = 0.33f;
    moveFrames = ImageHandler::LoadAnimationFrames("arcane_warden", 5);
}

std::string Arcane_Warden::getName() const {
    return "Arcane Warden";
}

void Arcane_Warden::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (type == "Magic") {
        health -= static_cast<int> (amount * 0.5);
    } else {
        health -= amount;
    }

    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Arcane_Warden::spawn() {
    Vector2 spawnPos = getPosition();
    std::shared_ptr<Enemy> enemy = std::make_shared<Arcane_Shell>();
    enemy->setPosition(spawnPos);
    enemy->setCurrentTarget(currentTarget);
    enemies.push_back(std::move(enemy));
}

void Arcane_Warden::draw() const {
    Texture2D frame = moveFrames[currentFrame];
    float diameter = hitboxRadius * 2.0f;

    Rectangle dest = {
        position.x,
        position.y,
        diameter,
        diameter
    };

    Vector2 origin = {
        diameter / 2.0f,
        diameter / 2.0f
    };

    float angleDeg = 0.0f;

    if (currentTarget < trackPoints.size()) {
        Vector2 dir = Vector2Subtract(trackPoints[currentTarget], position);
        float angleRad = atan2f(dir.y, dir.x);
        angleDeg = angleRad * (180.0f / PI);

        // Flip horizontally when moving left
        if (angleDeg > 90.0f || angleDeg < -90.0f) {
            angleDeg += 180.0f;
            // Flip source rect too
            Rectangle source = {
                0.0f, 0.0f,
                (float)-frame.width, // flip horizontally
                (float)frame.height
            };
            DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
            return;
        }
    }

    // Default source rect (not flipped)
    Rectangle source = {
        0.0f, 0.0f,
        (float)frame.width,
        (float)frame.height
    };

    DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
}

Null_Imp::Null_Imp() : Enemy(4000, 65.0f, 15.0f) {}

std::string Null_Imp::getName() const {
    return "Null Imp";
}

void Null_Imp::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (type == "Magic") {
        health -= static_cast<int> (amount * 1.5);
    } else {
        health -= amount;
    }
    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Null_Imp::draw() const {
    DrawCircleV(position, hitboxRadius, LIGHTGRAY);
}

The_Host::The_Host() : SpawnableEnemy(12000, 40.0f, 30.0f, 1.0f, 4.0f, 3) {}

std::string The_Host::getName() const {
    return "The Host";
}

void The_Host::takeDamage(int amount, const std::string& type, const std::string& targeting) {

    revertSpeed();
    health -= amount;

    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void The_Host::spawn() {

    Vector2 spawnPos = getPosition();
    int random = GetRandomValue(1, 10);

    std::shared_ptr<Enemy> enemy;

    if (random <= 2) {
        enemy = std::make_shared<Null_Imp>();
    } else if (random <= 5) {
        enemy = std::make_shared<Ravager>();
    } else {
        enemy = std::make_shared<Husk>();
    }

    enemy->setPosition(spawnPos);
    enemy->setCurrentTarget(currentTarget);
    enemies.push_back(std::move(enemy));
}

void The_Host::draw() const {
    DrawCircleV(position, hitboxRadius, MAGENTA);
}

Bulwark::Bulwark() : Enemy(10000, 30.0f, 25.0f) {}

std::string Bulwark::getName() const {
    return "Bulwark";
}

void Bulwark::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (targeting == "Area of Effect") {
        health -= static_cast<int> (amount * 0.5);
    } else {
        health -= amount;
    }

    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Bulwark::draw() const {
    DrawCircleV(position, hitboxRadius, WHITE);
}

Prime_Sludge::Prime_Sludge() : Enemy(6000, 100.0f, 20.0f) {
    frameSpeed = 0.33f;
    moveFrames = ImageHandler::LoadAnimationFrames("prime_sludge", 2);
}

std::string Prime_Sludge::getName() const {
    return "Prime Sludge";
}

void Prime_Sludge::takeDamage(int amount, const std::string& type, const std::string& targeting) {
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

void Prime_Sludge::spawn() {
    Vector2 spawnPos = getPosition();
    std::shared_ptr<Enemy> enemy = std::make_shared<Sludge>();
    enemy->setPosition(spawnPos);
    enemy->setCurrentTarget(currentTarget);
    deathSpawns.push_back(std::move(enemy));
}

void Prime_Sludge::draw() const {
    Texture2D frame = moveFrames[currentFrame];
    float diameter = hitboxRadius * 2.0f;

    Rectangle dest = {
        position.x,
        position.y,
        diameter,
        diameter
    };

    Vector2 origin = {
        diameter / 2.0f,
        diameter / 2.0f
    };

    float angleDeg = 0.0f;

    if (currentTarget < trackPoints.size()) {
        Vector2 dir = Vector2Subtract(trackPoints[currentTarget], position);
        float angleRad = atan2f(dir.y, dir.x);
        angleDeg = angleRad * (180.0f / PI);

        // Flip horizontally when moving left
        if (angleDeg > 90.0f || angleDeg < -90.0f) {
            angleDeg += 180.0f;
            // Flip source rect too
            Rectangle source = {
                0.0f, 0.0f,
                (float)-frame.width, // flip horizontally
                (float)frame.height
            };
            DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
            return;
        }
    }

    // Default source rect (not flipped)
    Rectangle source = {
        0.0f, 0.0f,
        (float)frame.width,
        (float)frame.height
    };

    DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
}

Big_Slime::Big_Slime() : Enemy(50000, 25.0f, 40.0f) {
    moveFrames = ImageHandler::LoadAnimationFrames("slime", 8);
}

std::string Big_Slime::getName() const {
    return "Big Slime";
}

void Big_Slime::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    health -= amount;
    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Big_Slime::draw() const {
    Texture2D frame = moveFrames[currentFrame];
    float diameter = hitboxRadius * 2.0f;

    Rectangle dest = {
        position.x,
        position.y,
        diameter,
        diameter
    };

    Vector2 origin = {
        diameter / 2.0f,
        diameter / 2.0f
    };

    float angleDeg = 0.0f;

    if (currentTarget < trackPoints.size()) {
        Vector2 dir = Vector2Subtract(trackPoints[currentTarget], position);
        float angleRad = atan2f(dir.y, dir.x);
        angleDeg = angleRad * (180.0f / PI);

        // Flip horizontally when moving left
        if (angleDeg > 90.0f || angleDeg < -90.0f) {
            angleDeg += 180.0f;
            // Flip source rect too
            Rectangle source = {
                0.0f, 0.0f,
                (float)-frame.width, // flip horizontally
                (float)frame.height
            };
            DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
            return;
        }
    }

    // Default source rect (not flipped)
    Rectangle source = {
        0.0f, 0.0f,
        (float)frame.width,
        (float)frame.height
    };

    DrawTexturePro(frame, source, dest, origin, angleDeg, WHITE);
}

Fractured_King::Fractured_King() : Enemy(600000, 20.0f, 30.0f) {}

void Fractured_King::update(float deltaTime, const std::vector<Vector2>& track) {
    
    shieldChangeTimer += GetFrameTime();

    if (!stunned) {
        Enemy::update(deltaTime, track); // Might have to change this to ensure enemy still gets burned when stunned
    } else if (stunned) {
        stunTimer += deltaTime;
        if (stunTimer >= stunDuration) {
            stunned = false;
            stunTimer = 0.0f;
            shieldChangeTimer = 0.0f;
        }
        return;
    }

    if (shield == "None" && shieldChangeTimer >= shieldChangeCooldown) {
        int val = GetRandomValue(1, 4);
        switch (val) {
            case 1:
                shield = "Physical";
                shieldColor = GRAY;
                break;
            case 2:
                shield = "Magic";
                shieldColor = PINK;
                break;
            case 3:
                shield = "Fire";
                shieldColor = ORANGE;
                break;
            case 4:
                shield = "Air";
                shieldColor = WHITE;
                break;
        }
        shieldChangeTimer = 0.0f;
        currentShieldAmount = shieldAmount;
    }

    if (shield != "None" && shieldChangeTimer >= shieldDuration) {
        shield = "None";
        shieldChangeTimer = 0.0f;
        speed += 5;
    }

}

std::string Fractured_King::getName() const {
    return "Fractured King";
}

void Fractured_King::takeDamage(int amount, const std::string& type, const std::string& targeting) {
    if (shield == "None") {
        if (stunned) {
            health -= static_cast<int> (amount * 2);
        } else {
            health -= static_cast<int> (amount * 0.75);
        }
    }
    if (shield != "None") {
        if (shield == type) {
            currentShieldAmount -= amount;
            if (currentShieldAmount <= 0) {
                stunned = true;
                shield = "None";
            }
        }
        health -= static_cast<int> (amount * 0.5);
    } 
    if (health <= 0) {
        alive = false;
        health = 0;
    }
}

void Fractured_King::draw() const {
    DrawCircleV(position, hitboxRadius, RED);
    if (shield != "None") {
        DrawCircleLinesV(position, hitboxRadius, shieldColor);
    }
}

std::string Fractured_King::getShield() const {
    return shield;
}

int Fractured_King::getTotalShield() const {
    return shieldAmount;
}

int Fractured_King::getShieldAmount() const {
    return currentShieldAmount;
}

Color Fractured_King::getShieldColor() const {
    return shieldColor;
}