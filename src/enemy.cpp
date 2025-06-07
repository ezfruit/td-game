#include "raylib.h"
#include "raymath.h"
#include "enemy.h"

Enemy::Enemy(int health, float speed) : health(health), speed(speed), baseSpeed(speed) {}

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
    if (currentTarget >= track.size()) return; // Already reached end

    if (deltaTime > 0.05f) deltaTime = 0.05f;

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
    }
    if (health <= 0) {
        alive = false;
    }
}