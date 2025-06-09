#include "raylib.h"
#include "raymath.h"
#include "projectile.h"
#include "tower.h"

Projectile::Projectile(Vector2 pos, Vector2 dir, float spd, int dmg, std::string type, std::weak_ptr<Tower> source, int pierceCount, float AoERadius) : 
    position(pos), direction(Vector2Normalize(dir)), speed(spd), damage(dmg), type(type), sourceTower(source), pierceCount(pierceCount), AoERadius(AoERadius) {}

void Projectile::update(float deltaTime, std::weak_ptr<Tower> source) {
    position = Vector2Add(position, Vector2Scale(direction, speed * deltaTime));

    if (auto src = source.lock()) { // lock to shared_ptr and check if valid
        float distance = Vector2Distance(getPosition(), src->getPosition());

        if (position.x < 0 || position.x > GetScreenWidth() ||
            position.y < 0 || position.y > GetScreenHeight() ||
            distance > src->getProjectileRange()) {
            active = false;
        }
    } else {
        // source tower is gone (expired), so deactivate projectile
        active = false;
    }
}

void Projectile::draw() const {
    DrawCircleV(position, 5, RED);
}

void Projectile::drawExplosion() const {
    DrawCircle(getPosition().x, getPosition().y, getAOERadius(), Fade(ORANGE, 0.5f));
}

bool Projectile::isActive() const { 
    return active; 
}
    
void Projectile::deactivate() {
    active = false; 
}

Vector2 Projectile::getPosition() const { 
    return position; 
}
    
int Projectile::getDamage() const { 
    return damage; 
}
    
std::string Projectile::getDamageType() const { 
    return type; 
}

std::weak_ptr<Tower> Projectile::getSourceTower() const { 
    return sourceTower; 
}

void Projectile::setPierceCount(int count) {
    pierceCount = count;
}
        
int Projectile::getPierceCount() const {
    return pierceCount;
}

void Projectile::setAOERadius(float radius) {
    AoERadius = radius;
}

float Projectile::getAOERadius() const {
    return AoERadius;
}
        
bool Projectile::hasHit(Enemy* enemy) const {
    return hitEnemies.find(enemy) != hitEnemies.end();
}
void Projectile::markHit(Enemy* enemy) {
    hitEnemies.insert(enemy);
    pierceCount--;

    if (pierceCount <= 0) {
        active = false;
    }
}


