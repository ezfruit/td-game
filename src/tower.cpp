#include "tower.h"

Tower::Tower(int range, int damage, float attackSpeed, std::string targeting, int cost, Vector2 position) :
            range(range), damage(damage), attackSpeed(attackSpeed), targeting(targeting), cost(cost), position(position) {}

bool Tower::canShoot() const {
    return attackCooldown <= 0.0f;
}

Vector2 Tower::getPosition() const {
    return position;
}

std::string Tower::getName() const {
    return name;
}

int Tower::getRange() const {
    return range;
}

int Tower::getDamage() const {
    return damage;
}

void Tower::setTotalDamageDealt(int dmg) {
    totalDamageDealt += dmg;
}

int Tower::getTotalDamageDealt() const {
    return totalDamageDealt;
}

void Tower::setValue(int val) {
    value += (val / 2);
}

int Tower::getValue() const {
    return value;
}

int Tower::getLevel() const {
    return level;
}

float Tower::getProjectileRange() const {
    return projectileRange;
}

Archer::Archer(Vector2 pos) : Tower(150, 2, 0.8f, "single", 200, pos) {
    name = "Archer";
    type = "Physical";
    value = 100;
    projectileSpeed = 400.0f;
    projectileRange = 400.0f;
}

void Archer::attack(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles) {
    attackCooldown -= deltaTime;
    if (attackCooldown > 0) return;

    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        float distance = Vector2Distance(getPosition(), enemy->getPosition());
        if (distance <= range) {
            Vector2 dir = Vector2Subtract(enemy->getPosition(), getPosition());
            projectiles.emplace_back(getPosition(), dir, projectileSpeed, damage, type, shared_from_this(), pierceCount, AoERadius);
            attackCooldown = 1.0f / attackSpeed;
            break;
        }
    }
}

void Archer::upgrade(int upgCost) {
    level += 1;
    if (level > 5) {
        level = 5;
        return;
    } else {
        value += (upgCost / 2);
    }
    switch (level) {
        case 2:
            damage += 2;
            range += 25;
            projectileSpeed += 100;
            break;
        case 3:
            damage += 6;
            attackSpeed = 1.0;
            pierceCount = 2;
            break;
        case 4:
            damage += 15;
            attackSpeed = 1.5;
            projectileSpeed += 100;
            projectileRange += 500;
            break;
        case 5:
            damage += 25;
            range += 25;
            attackSpeed = 2;
            pierceCount = 4;
            projectileSpeed += 100;
            projectileRange += 500;
            break;
    }
}

Mage::Mage(Vector2 pos) : Tower(100, 3, 0.5, "AoE", 300, pos) {
    name = "Mage";
    type = "Magic";
    value = 150;
    projectileSpeed = 500.0f;
    projectileRange = 500.0f;
    AoERadius = 50.0f;
}

void Mage::attack(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles) {
    attackCooldown -= deltaTime;
    if (attackCooldown > 0) return;

    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        float distance = Vector2Distance(getPosition(), enemy->getPosition());
        if (distance <= range) {
            Vector2 dir = Vector2Subtract(enemy->getPosition(), getPosition());
            projectiles.emplace_back(getPosition(), dir, projectileSpeed, damage, type, shared_from_this(), pierceCount, AoERadius);
            attackCooldown = 1.0f / attackSpeed;
            break;
        }
    }
}

void Mage::upgrade(int upgCost) {
    level += 1;
    if (level > 5) {
        level = 5;
        return;
    } else {
        value += (upgCost / 2);
    }
    switch (level) {
        case 2:
            damage += 3;
            range += 25;
            break;
        case 3:
            damage += 6;
            AoERadius += 10;
            break;
        case 4:
            damage += 8;
            range += 25;
            attackSpeed = 1.0;
            break;
        case 5:
            damage += 24;
            range += 25;
            attackSpeed = 1.5;
            AoERadius += 25;
            break;
    }
}