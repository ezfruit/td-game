#include "raylib.h"
#include "raymath.h"
#include "projectile.h"
#include "tower.h"
#include "sounds.h"

Projectile::Projectile(Vector2 pos, Vector2 dir, float spd, int dmg, std::string type, std::string targeting, std::weak_ptr<Tower> source, int pierceCount, float AoERadius) : 
    position(pos), direction(Vector2Normalize(dir)), speed(spd), damage(dmg), type(type), targeting(targeting), sourceTower(source), pierceCount(pierceCount), AoERadius(AoERadius) {}

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

// Override later when separating projectiles (arrows, fireball, etc.)
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

std::string Projectile::getDamageTargeting() const {
    return targeting;
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

void Projectile::ApplyAOEDamage(Projectile& projectile, Vector2 center, float radius, int damage, std::string type) {
    PlaySound(SoundManager::explosion);
    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        float dist = Vector2Distance(enemy->getPosition(), center) - enemy->getRadius();
        if (dist <= radius) {
            int prevHealth = enemy->getHealth();
            enemy->takeDamage(damage, type, targeting);
            projectile.markHit(enemy.get());
            int curHealth = enemy->getHealth();
            int damageDealt = prevHealth - curHealth;
            if (auto shooter = projectile.getSourceTower().lock()) {
                shooter->setTotalDamageDealt(damageDealt);
            }
            playerMoney += damageDealt;
        }
    }
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

Arrow::Arrow(Vector2 pos, Vector2 dir, float spd, int dmg, std::string type, std::string targeting, std::weak_ptr<Tower> source, int pierceCount, float AoERadius) 
        : Projectile(pos, dir, spd, dmg, type, targeting, source, pierceCount, AoERadius) {}

void Arrow::draw() const {
    
    float shaftLength = 20.0f;
    float shaftWidth = 2.0f;
    float headLength = 8.0f;
    float headWidth = 6.0f;

    // Calculate shaft end
    Vector2 shaftEnd = {
        position.x + direction.x * shaftLength,
        position.y + direction.y * shaftLength
    };

    // Draw shaft (thin line)
    DrawLineEx(position, shaftEnd, shaftWidth, DARKBROWN);

    // Calculate perpendicular vector for arrowhead base
    Vector2 perp = { -direction.y, direction.x }; // 90-degree rotation

    // Arrowhead tip (extends beyond shaft)
    Vector2 tip = {
        shaftEnd.x + direction.x * headLength,
        shaftEnd.y + direction.y * headLength
    };

    // Base corners of the arrowhead triangle
    Vector2 baseLeft = {
        shaftEnd.x + perp.x * (headWidth / 2),
        shaftEnd.y + perp.y * (headWidth / 2)
    };

    Vector2 baseRight = {
        shaftEnd.x - perp.x * (headWidth / 2),
        shaftEnd.y - perp.y * (headWidth / 2)
    };

    // Draw triangle arrowhead
    DrawTriangle(tip, baseLeft, baseRight, BROWN);
}

Fireball::Fireball(Vector2 pos, Vector2 dir, float spd, int dmg, std::string type, std::string targeting, std::weak_ptr<Tower> source, int pierceCount, float AoERadius)
        : Projectile(pos, dir, spd, dmg, type, targeting, source, pierceCount, AoERadius) {}

void Fireball::draw() const {
    // Core position
    float radius = 8.0f;

    // Glowing outer aura (drawn behind the core)
    DrawCircleV(position, radius + 6, ColorAlpha(ORANGE, 0.3f));  // light aura
    DrawCircleV(position, radius + 3, ColorAlpha(RED, 0.5f));     // mid aura

    // Core
    DrawCircleV(position, radius, RED);

    // Center sparkle or flame core
    DrawCircleV(position, radius / 2, YELLOW);
}

Flames::Flames(Vector2 pos, Vector2 dir, float spd, int dmg, std::string type, std::string targeting, std::weak_ptr<Tower> source, int pierceCount, float AoERadius) 
        : Projectile(pos, dir, spd, dmg, type, targeting, source, pierceCount, AoERadius) {}

void Flames::setTarget(std::shared_ptr<Enemy> enemy) {
    target = enemy;
}

void Flames::setBurnDelay(float delay) {
    burnDelay = delay;
}

void Flames::setBurnDamage(int dmg) {
    burnDPS = dmg;
}

void Flames::setBurnDuration(float duration) {
    burnDuration = duration;
}

void Flames::setSlowEffect(float slow) {
    slowEffect = slow;
}

bool Flames::hasReachedTarget() const {
    if (!target) {
        return false;
    }

    float distance = Vector2Distance(position, target->getPosition());
    return distance <= target->getRadius();
}

void Flames::update(float deltaTime, std::weak_ptr<Tower> source) {
    Projectile::update(deltaTime, source);

    if (target && hasReachedTarget()) {
        target->applyBurn(burnDelay, burnDPS, burnDuration, slowEffect, source);
        deactivate();
    }
}
