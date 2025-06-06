#include "tower.h"

Tower::Tower(int range, int damage, double attack_speed, std::string targeting, int cost, Vector2 position) :
            range(range), damage(damage), attack_speed(attack_speed), targeting(targeting), cost(cost), position(position) {}

Vector2 Tower::getPosition() const {
    return position;
}

Archer::Archer(Vector2 pos) : Tower(200, 2, 1.0, "single", 200, pos) {}

void Archer::attack(std::vector<Enemy*>& enemies) {

}

void Archer::upgrade() {
    upgrade_level += 1;
}

Mage::Mage(Vector2 pos) : Tower(400, 3, 0.8, "AoE", 300, pos) {}

void Mage::attack(std::vector<Enemy*>& enemies) {

}

void Mage::upgrade() {
    upgrade_level += 1;
}
