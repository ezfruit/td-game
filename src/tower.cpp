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

Archer::Archer(Vector2 pos) : Tower(150, 2, 0.8f, "single", 200, pos) {
    name = "Archer";
    type = "Physical";
    value = 100;
}

void Archer::attack(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles) {
    attackCooldown -= deltaTime;
    if (attackCooldown > 0) return;

    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        float distance = Vector2Distance(getPosition(), enemy->getPosition());
        if (distance <= range) {
            int prevHealth = enemy->getHealth();
            enemy->takeDamage(damage, type);
            int curHealth = enemy->getHealth();
            int damageDealt = prevHealth - curHealth;
            setTotalDamageDealt(damageDealt);
            playerMoney += damageDealt;
            attackCooldown = 1.0f / attackSpeed;
            break; // only attack one enemy
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
            break;
        case 3:
            damage += 6;
            targeting = "Double";
            break;
        case 4:
            damage += 15;
            attackSpeed = 1.5;
            break;
        case 5:
            damage += 25;
            attackSpeed = 2;
            targeting = "Quadruple";
            break;
    }
}

Mage::Mage(Vector2 pos) : Tower(100, 3, 0.8, "AoE", 300, pos) {
    name = "Mage";
    type = "Magic";
    value = 150;
}

void Mage::attack(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles) {

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
            // Slightly increase AoE
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
            // Increase AoE
            break;
    }
}