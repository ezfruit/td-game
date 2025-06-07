#include "tower.h"

Tower::Tower(int range, int damage, double attack_speed, std::string targeting, int cost, Vector2 position) :
            range(range), damage(damage), attack_speed(attack_speed), targeting(targeting), cost(cost), position(position) {}

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

Archer::Archer(Vector2 pos) : Tower(150, 2, 1.0, "single", 200, pos) {
    name = "Archer";
    value = 100;
}

void Archer::attack(std::vector<Enemy*>& enemies) {

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
    value = 150;
}

void Mage::attack(std::vector<Enemy*>& enemies) {

}

void Mage::upgrade(int upgCost) {
    level += 1;
    if (level > 5) {
        level = 5;
    } else {
        value += (upgCost / 2);
    }
}