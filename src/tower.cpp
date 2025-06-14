#include "tower.h"
#include "sounds.h"

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

std::string Tower::getTargeting() const {
    return targeting;
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

Archer::Archer(Vector2 pos) : Tower(150, 2, 0.8f, "Pierce", 200, pos) {
    name = "Archer";
    type = "Physical";
    value = cost / 2;
    projectileSpeed = 400.0f;
    projectileRange = 400.0f;
}

void Archer::attack(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::shared_ptr<Projectile>>& projectiles) {
    attackCooldown -= deltaTime;
    if (attackCooldown > 0) return;

    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        float distance = Vector2Distance(getPosition(), enemy->getPosition()) - enemy->getRadius();
        if (distance <= range) {
            PlaySound(SoundManager::arrow_fly);
            Vector2 dir = Vector2Subtract(enemy->getPosition(), getPosition());
            projectiles.emplace_back(std::make_shared<Projectile>(getPosition(), dir, projectileSpeed, damage, type, shared_from_this(), pierceCount, AoERadius));
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
            range += 25;
            attackSpeed = 1.0f;
            projectileSpeed += 100;
            break;
        case 3:
            damage += 3;
            pierceCount = 2;
            projectileSpeed += 100;
            projectileRange += 250;
            break;
        case 4:
            damage += 10;
            attackSpeed = 1.5f;
            projectileSpeed += 200;
            projectileRange += 500;
            break;
        case 5:
            damage += 25;
            range += 25;
            attackSpeed = 2;
            pierceCount = 4;
            projectileSpeed += 200;
            projectileRange += 500;
            break;
    }
}

Mage::Mage(Vector2 pos) : Tower(100, 3, 0.5, "Area of Effect", 300, pos) {
    name = "Mage";
    type = "Magic";
    value = cost / 2;
    projectileSpeed = 500.0f;
    projectileRange = 500.0f;
    AoERadius = 50.0f;
}

void Mage::attack(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::shared_ptr<Projectile>>& projectiles) {
    attackCooldown -= deltaTime;
    if (attackCooldown > 0) return;

    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        float distance = Vector2Distance(getPosition(), enemy->getPosition()) - enemy->getRadius();
        if (distance <= range) {
            PlaySound(SoundManager::fireball);
            Vector2 dir = Vector2Subtract(enemy->getPosition(), getPosition());
            projectiles.emplace_back(std::make_shared<Projectile>(getPosition(), dir, projectileSpeed, damage, type, shared_from_this(), pierceCount, AoERadius));
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
            damage += 1;
            range += 25;
            break;
        case 3:
            damage += 3;
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

Torcher::Torcher(Vector2 pos) : Tower(75, 1, 1.0, "Single", 700, pos) {
    name = "Torcher";
    type = "Fire";
    value = cost / 2;
    projectileSpeed = 1000.0f;
    projectileRange = range;
}

bool Torcher::IsInRange(std::shared_ptr<Enemy> enemy) {
    float distance = Vector2Distance(getPosition(), enemy->getPosition()) - enemy->getRadius();
    if (distance <= range) {
        return true;
    }
    return false;
}

std::shared_ptr<Enemy> Torcher::FindUnburnedTarget(std::vector<std::shared_ptr<Enemy>>& enemies) {
    for (auto& enemy : enemies) {
        if (!enemy->isBurning() && IsInRange(enemy)) {
            return enemy;
        }
    }
    return nullptr; // All are burning
}

void Torcher::FireAt(std::shared_ptr<Enemy> enemy) {
    Vector2 dir = Vector2Subtract(enemy->getPosition(), getPosition());
    auto flamesProj = std::make_shared<Flames>(getPosition(), dir, projectileSpeed, damage, type, shared_from_this(), pierceCount, AoERadius);
    flamesProj->setTarget(enemy);
    flamesProj->setBurnDelay(burnDelay);
    flamesProj->setBurnDamage(damage);
    flamesProj->setBurnDuration(burnDuration);
    flamesProj->setSlowEffect(slowEffect);
    projectiles.emplace_back(flamesProj);
}

void Torcher::attack(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::shared_ptr<Projectile>>& projectiles) {
    timeSinceLastFire += deltaTime;

    if (timeSinceLastFire >= fireCooldown) {
        auto target = FindUnburnedTarget(enemies);
        if (target) {
            PlaySound(SoundManager::torcher);
            FireAt(target);
            timeSinceLastFire = 0.0f;
        }
    }
}

void Torcher::upgrade(int upgCost) {
    level += 1;
    if (level > 5) {
        level = 5;
        return;
    } else {
        value += (upgCost / 2);
    }
    switch (level) {
        case 2:
            damage += 1;
            range += 25;
            projectileRange = range;
            break;
        case 3:
            damage += 3;
            fireCooldown = 0.75f;
            slowEffect = 0.8;
            break;
        case 4:
            burnDuration += 2;
            burnDelay = 0.5;
            break;
        case 5:
            damage += 15;
            slowEffect = 0.6;
            range += 25;
            projectileRange = range;
            burnDuration += 2;
            burnDelay = 0.25;
            fireCooldown = 0.5f;
            break;
    }
}

Stormshaper::Stormshaper(Vector2 pos) : Tower(300, 30, 0.2, "Area of Effect", 3000, pos) {
    name = "Stormshaper";
    type = "Air";
    value = cost / 2;
    projectileSpeed = 4000.0f;
    projectileRange = range * 2;
    AoERadius = 10.0f;
}

void Stormshaper::attack(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::shared_ptr<Projectile>>& projectiles) {
    attackCooldown -= deltaTime;
    if (attackCooldown > 0) return;

    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        float distance = Vector2Distance(getPosition(), enemy->getPosition()) - enemy->getRadius();
        if (distance <= range) {
            Vector2 dir = Vector2Subtract(enemy->getPosition(), getPosition());
            projectiles.emplace_back(std::make_shared<Projectile>(getPosition(), dir, projectileSpeed, damage, type, shared_from_this(), pierceCount, AoERadius));
            attackCooldown = 1.0f / attackSpeed;
            break;
        }
    }
}

void Stormshaper::upgrade(int upgCost) {
    level += 1;
    if (level > 5) {
        level = 5;
        return;
    } else {
        value += (upgCost / 2);
    }
    switch (level) {
        case 2:
            attackSpeed = 0.4;
            break;
        case 3:
            damage += 20;
            range += 50;
            break;
        case 4:
            damage += 20;
            attackSpeed = 0.5;
            AoERadius += 5;
            break;
        case 5:
            damage += 50;
            range += 50;
            attackSpeed = 0.75;
            AoERadius += 5;
            break;
    }
}