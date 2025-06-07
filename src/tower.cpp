#include "tower.h"

Tower::Tower(int range, int damage, float attackSpeed, std::string targeting, int cost, Vector2 position) :
            range(range), damage(damage), attackSpeed(attackSpeed), targeting(targeting), cost(cost), position(position) {}

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

void Archer::attack(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
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
    } else {
        value += (upgCost / 2);
    }
}

Mage::Mage(Vector2 pos) : Tower(100, 3, 0.8, "AoE", 300, pos) {
    name = "Mage";
    type = "Magic";
    value = 150;
}

void Mage::attack(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {

}

void Mage::upgrade(int upgCost) {
    level += 1;
    if (level > 5) {
        level = 5;
    } else {
        value += (upgCost / 2);
    }
}