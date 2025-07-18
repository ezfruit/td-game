#include "tower.h"
#include "sounds.h"
#include "effects.h"
#include "images.h"

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

void Tower::setDamageMultiplier(float multiplier) {
    damageMultiplier = multiplier;
}

float Tower::getDamageMultiplier() const {
    return damageMultiplier;
}

void Tower::setAttackSpeedMultiplier(float multiplier) {
    attackSpeedMultiplier = multiplier;
}

float Tower::getAttackSpeedMultiplier() const {
    return attackSpeedMultiplier;
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

void Tower::resetBuffs() {
    war_drummer_buff = false;
    damageMultiplier = 1.0f;
    attackSpeedMultiplier = 1.0f;
}

void Tower::setWarDrummerBuff() {
    war_drummer_buff = true;
}

bool Tower::getWarDrummerBuff() const {
    return war_drummer_buff;
}

bool Tower::IsInRange(std::shared_ptr<Enemy> enemy) {
    float distance = Vector2Distance(getPosition(), enemy->getPosition()) - enemy->getRadius();
    if (distance <= range) {
        return true;
    }
    return false;
}

std::shared_ptr<Enemy> Tower::FindFirstTarget(std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track) {
    std::shared_ptr<Enemy> target = nullptr;
    float maxProgress = -1.0f;

    for (const auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        float progress = enemy->getPathProgress(track);
        if (IsInRange(enemy) && progress > maxProgress) {
            maxProgress = progress;
            target = enemy;
        }
    }

    return target;
}

std::shared_ptr<Enemy> Tower::FindLastTarget(std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track) {
    std::shared_ptr<Enemy> target = nullptr;
    float minProgress = std::numeric_limits<float>::max();

    for (const auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        float progress = enemy->getPathProgress(track);
        if (IsInRange(enemy) && progress < minProgress) {
            minProgress = progress;
            target = enemy;
        }
    }

    return target;
}

std::shared_ptr<Enemy> Tower::FindStrongestTarget(std::vector<std::shared_ptr<Enemy>>& enemies) {
    int highestHealth = 0;
    std::shared_ptr<Enemy> target = nullptr;
    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        if (IsInRange(enemy) && enemy->getHealth() > highestHealth) {
            highestHealth = enemy->getHealth();
            target = enemy;
        }
    }
    return target;
}

std::shared_ptr<Enemy> Tower::FindWeakestTarget(std::vector<std::shared_ptr<Enemy>>& enemies) {
    int lowestHealth = std::numeric_limits<int>::max();
    std::shared_ptr<Enemy> target = nullptr;
    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        if (IsInRange(enemy) && enemy->getHealth() < lowestHealth) {
            lowestHealth = enemy->getHealth();
            target = enemy;
        }
    }
    return target;
}

void Tower::ApplyAOEDamage(Vector2 center, float radius, int damage, std::string type) {
    PlaySound(SoundManager::explosion);
    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        float dist = Vector2Distance(enemy->getPosition(), center) - enemy->getRadius();
        if (dist <= radius) {
            int prevHealth = enemy->getHealth();
            enemy->takeDamage(damage, type, targeting);
            int curHealth = enemy->getHealth();
            int damageDealt = prevHealth - curHealth;
            setTotalDamageDealt(damageDealt);
            playerMoney += damageDealt;
        }
    }
    // Add new enemies after the loop
    enemies.insert(enemies.end(), std::make_move_iterator(deathSpawns.begin()), std::make_move_iterator(deathSpawns.end()));
    deathSpawns.clear();
}

float Tower::getProjectileRange() const {
    return projectileRange;
}

std::shared_ptr<Enemy> Tower::EnemyToShoot(TargetMode mode, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track) {
    std::shared_ptr<Enemy> target = nullptr;
    if (mode == FIRST) {
        target = FindFirstTarget(enemies, track);
    } else if (mode == LAST) {
        target = FindLastTarget(enemies, track);
    } else if (mode == STRONG) {
        target = FindStrongestTarget(enemies);
    } else if (mode == WEAK) {
        target = FindWeakestTarget(enemies);
    }
    return target;
}

void Tower::setTargetMode(TargetMode newMode) {
    mode = newMode;
}

TargetMode Tower::getTargetMode() const {
    return mode;
}

void Tower::unloadFrames() {
    for (auto& frame : animationFrames) {
        UnloadTexture(frame);
    }
    animationFrames.clear();
}

Archer::Archer(Vector2 pos) : Tower(150, 2, 0.8f, "Pierce", 200, pos) {
    name = "Archer";
    type = "Physical";
    value = cost / 2;
    projectileSpeed = 400.0f;
    projectileRange = 400.0f;
    pierceCount = 2;
    animationFrames = ImageHandler::LoadAnimationFrames("archer", 10);
}

void Archer::update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track, std::vector<std::shared_ptr<Projectile>>& projectiles) {

    attackCooldown -= deltaTime;
    shootTimer -= deltaTime; // For shoot vs. idle animation

    if (attackCooldown > 0) return;

    auto target = EnemyToShoot(mode, enemies, track);

    if (target) {
        Vector2 toTarget = Vector2Subtract(target->getPosition(), position);
        float angleRad = atan2f(toTarget.y, toTarget.x);
        float angleDeg = angleRad * RAD2DEG;
        isShooting = true;

        // Flip based on left/right of tower
        facingLeft = (toTarget.x < 0);

        rotationAngle = facingLeft ? angleDeg + 180.0f : angleDeg;

        shootTimer = shootFlashDuration;

        PlaySound(SoundManager::arrow_fly);
        Vector2 dir = Vector2Subtract(target->getPosition(), getPosition());

        int actualDamage = static_cast<int>(std::ceil(damage * damageMultiplier));
        float actualAttackSpeed = attackSpeed * attackSpeedMultiplier;

        projectiles.emplace_back(std::make_shared<Arrow>(getPosition(), dir, projectileSpeed, actualDamage, type, targeting, shared_from_this(), pierceCount, AoERadius));
        attackCooldown = 1.0f / actualAttackSpeed;
    }

    isShooting = shootTimer > 0.0f; // Goes back to idle position after a certain time

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
            shootFrame += 2;
            break;
        case 3:
            damage += 3;
            pierceCount = 3;
            projectileSpeed += 100;
            projectileRange += 250;
            shootFrame += 2;
            break;
        case 4:
            damage += 10;
            attackSpeed = 1.25f;
            projectileSpeed += 200;
            projectileRange += 500;
            shootFrame += 2;
            break;
        case 5:
            damage += 25;
            range += 25;
            attackSpeed = 1.5f;
            pierceCount = 4;
            projectileSpeed += 200;
            projectileRange += 500;
            shootFrame += 2;
            break;
    }
}

void Archer::draw() const {
    Texture2D frame = isShooting ? animationFrames[shootFrame] : animationFrames[shootFrame + 1];

    Rectangle source = {
        0.0f, 0.0f,
        static_cast<float>(frame.width) * (facingLeft ? -1.0f : 1.0f),
        static_cast<float>(frame.height)
    };

    Rectangle dest = {
        position.x,
        position.y,
        TOWER_SIZE,
        TOWER_SIZE
    };

    Vector2 origin = {
        TOWER_SIZE / 2.0f,
        TOWER_SIZE / 2.0f
    };

    DrawTexturePro(frame, source, dest, origin, rotationAngle, WHITE);
}

void Archer::showUpgradeInfo() const {

    int infoX = GetScreenWidth() / 2 + 270;
    int infoY = GetScreenHeight() - 160;

    int name_font_size = 20;
    int stat_font_size = 18;

    switch (level) {
        case 1:
            DrawText("Training", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Range", infoX + 10, infoY + 40, stat_font_size, WHITE);
            DrawText("+ Attack Speed", infoX + 10, infoY + 70, stat_font_size, WHITE);
            DrawText("+ Arrow Speed", infoX + 10, infoY + 100, stat_font_size, WHITE);
            break;
        case 2:
            DrawText("Enhanced Pierce", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Damage", infoX + 10, infoY + 40, stat_font_size, WHITE);
            DrawText("+ Pierce Count", infoX + 10, infoY + 70, stat_font_size, WHITE);
            DrawText("+ Arrow Speed", infoX + 10, infoY + 100, stat_font_size, WHITE);
            DrawText("+ Arrow Range", infoX + 10, infoY + 130, stat_font_size, WHITE);
            break;
        case 3:
            DrawText("Barbed Volley", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Damage", infoX + 10, infoY + 40, stat_font_size, WHITE);
            DrawText("+ Attack Speed", infoX + 10, infoY + 70, stat_font_size, WHITE);
            DrawText("+ Arrow Speed", infoX + 10, infoY + 100, stat_font_size, WHITE);
            DrawText("+ Arrow Range", infoX + 10, infoY + 130, stat_font_size, WHITE);
            break;
        case 4:
            DrawText("Arrowstorm", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ ALL Stats", infoX + 10, infoY + 40, stat_font_size,  WHITE);
            break;
        case 5:
            DrawText("Maxed", infoX + 10, infoY + 10, name_font_size, WHITE);
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
    animationFrames = ImageHandler::LoadAnimationFrames("mage", 10);
}

void Mage::update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track, std::vector<std::shared_ptr<Projectile>>& projectiles) {

    attackCooldown -= deltaTime;
    shootTimer -= deltaTime; // For shoot vs. idle animation

    if (attackCooldown > 0) return;

    auto target = EnemyToShoot(mode, enemies, track);

    if (target) {
        Vector2 toTarget = Vector2Subtract(target->getPosition(), position);
        float angleRad = atan2f(toTarget.y, toTarget.x);
        float angleDeg = angleRad * RAD2DEG;
        isShooting = true;

        // Flip based on left/right of tower
        facingLeft = (toTarget.x < 0);

        rotationAngle = facingLeft ? angleDeg + 180.0f : angleDeg;

        shootTimer = shootFlashDuration;

        PlaySound(SoundManager::fireball);
        Vector2 dir = Vector2Subtract(target->getPosition(), getPosition());

        int actualDamage = static_cast<int>(std::ceil(damage * damageMultiplier));
        float actualAttackSpeed = attackSpeed * attackSpeedMultiplier;

        projectiles.emplace_back(std::make_shared<Fireball>(getPosition(), dir, projectileSpeed, actualDamage, type, targeting, shared_from_this(), pierceCount, AoERadius));
        attackCooldown = 1.0f / actualAttackSpeed;
    }

    isShooting = shootTimer > 0.0f; // Goes back to idle position after a certain time
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
            shootFrame += 2;
            break;
        case 3:
            damage += 3;
            AoERadius += 10;
            shootFrame += 2;
            break;
        case 4:
            damage += 8;
            range += 25;
            attackSpeed = 1.0;
            shootFrame += 2;
            break;
        case 5:
            damage += 20;
            range += 25;
            attackSpeed = 1.25;
            AoERadius += 10;
            shootFrame += 2;
            break;
    }
}

void Mage::draw() const {
    Texture2D frame = isShooting ? animationFrames[shootFrame] : animationFrames[shootFrame + 1];

    Rectangle source = {
        0.0f, 0.0f,
        static_cast<float>(frame.width) * (facingLeft ? -1.0f : 1.0f),
        static_cast<float>(frame.height)
    };

    Rectangle dest = {
        position.x,
        position.y,
        TOWER_SIZE,
        TOWER_SIZE
    };

    Vector2 origin = {
        TOWER_SIZE / 2.0f,
        TOWER_SIZE / 2.0f
    };

    DrawTexturePro(frame, source, dest, origin, rotationAngle, WHITE);
    
}

void Mage::showUpgradeInfo() const {
    
    int infoX = GetScreenWidth() / 2 + 270;
    int infoY = GetScreenHeight() - 160;

    int name_font_size = 20;
    int stat_font_size = 18;

    switch (level) {
        case 1:
            DrawText("Mystic Focus", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Damage", infoX + 10, infoY + 40, stat_font_size, WHITE);
            DrawText("+ Range", infoX + 10, infoY + 70, stat_font_size, WHITE);
            break;
        case 2:
            DrawText("Bigger Fireballs", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Damage", infoX + 10, infoY + 40, stat_font_size, WHITE);
            DrawText("+ AoE Radius", infoX + 10, infoY + 70, stat_font_size, WHITE);
            break;
        case 3:
            DrawText("Mana Channel", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Damage", infoX + 10, infoY + 40, stat_font_size, WHITE);
            DrawText("+ Range", infoX + 10, infoY + 70, stat_font_size, WHITE);
            DrawText("+ Attack Speed", infoX + 10, infoY + 100, stat_font_size, WHITE);
            break;
        case 4:
            DrawText("Pyroclasm Obelisk", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Damage", infoX + 10, infoY + 40, stat_font_size,  WHITE);
            DrawText("+ Range", infoX + 10, infoY + 70, stat_font_size, WHITE);
            DrawText("+ Attack Speed", infoX + 10, infoY + 100, stat_font_size, WHITE);
            DrawText("+ AoE Radius", infoX + 10, infoY + 130, stat_font_size, WHITE);
            break;
        case 5:
            DrawText("Maxed", infoX + 10, infoY + 10, name_font_size, WHITE);
            break;
    }
}

Torcher::Torcher(Vector2 pos) : Tower(75, 1, 1.0, "Single", 700, pos) {
    name = "Torcher";
    type = "Fire";
    value = cost / 2;
    projectileSpeed = 1000.0f;
    projectileRange = range;
    shootFlashDuration = 5.0f;
    animationFrames = ImageHandler::LoadAnimationFrames("torcher", 30);
}

std::shared_ptr<Enemy> Torcher::FindUnburnedTarget(std::vector<std::shared_ptr<Enemy>>& enemies) {
    std::vector<std::shared_ptr<Enemy>> unburned;

    // Filter unburned enemies
    for (auto& enemy : enemies) {
        if (!enemy->isBurning()) {
            unburned.push_back(enemy);
        }
    }

    // Apply targeting mode on unburned only
    if (!unburned.empty()) {
        return EnemyToShoot(mode, unburned, trackPoints);
    }

    return nullptr; // No unburned targets found
}

void Torcher::FireAt(std::shared_ptr<Enemy> enemy, int actualDamage) {
    Vector2 dir = Vector2Subtract(enemy->getPosition(), getPosition());
    auto flamesProj = std::make_shared<Flames>(getPosition(), dir, projectileSpeed, damage, type, targeting, shared_from_this(), pierceCount, AoERadius);
    flamesProj->setTarget(enemy);
    flamesProj->setBurnDelay(burnDelay);
    flamesProj->setBurnDamage(actualDamage);
    flamesProj->setBurnDuration(burnDuration);
    flamesProj->setSlowEffect(slowEffect);
    projectiles.emplace_back(flamesProj);
}

void Torcher::update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track, std::vector<std::shared_ptr<Projectile>>& projectiles) {

    attackCooldown -= deltaTime;
    shootTimer -= deltaTime; // For shoot vs. idle animation

    isShooting = shootTimer > 0.0f;
    if (isShooting) {
        shootingFrameTimer += deltaTime;
        if (shootingFrameTimer >= shootingFrameDuration) {
            shootingFrameTimer -= shootingFrameDuration;
            shootingIndex = (shootingIndex + 1) % 5;
            currentShootingFrame = shootingIndex + (currentIdleFrame - 5);
        }
    } else {
        currentShootingFrame = currentIdleFrame - 5;
        shootingFrameTimer = 0.0f;
    }

    if (attackCooldown > 0) return;

    auto target = FindUnburnedTarget(enemies);

    if (target) {
        Vector2 toTarget = Vector2Subtract(target->getPosition(), position);
        float angleRad = atan2f(toTarget.y, toTarget.x);
        float angleDeg = angleRad * RAD2DEG;

        isShooting = true;
        facingLeft = (toTarget.x < 0);
        rotationAngle = facingLeft ? angleDeg + 180.0f : angleDeg;

        shootTimer = shootFlashDuration;

        PlaySound(SoundManager::torcher);

        int actualDamage = static_cast<int>(std::ceil(damage * damageMultiplier));
        float actualAttackSpeed = attackSpeed * attackSpeedMultiplier;

        FireAt(target, actualDamage);

        attackCooldown = 1.0f / actualAttackSpeed;
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

    currentIdleFrame += 6;

    switch (level) {
        case 2:
            damage += 1;
            range += 25;
            projectileRange = range;
            break;
        case 3:
            damage += 3;
            attackSpeed = 1.5f;
            slowEffect = 0.9;
            break;
        case 4:
            burnDuration += 2;
            burnDelay = 0.5;
            break;
        case 5:
            damage += 15;
            slowEffect = 0.75;
            range += 25;
            projectileRange = range;
            burnDuration += 2;
            burnDelay = 0.25;
            attackSpeed = 2.0f;
            break;
    }
}

void Torcher::draw() const {

    Texture2D frame;

    if (isShooting) {
        frame = animationFrames[currentShootingFrame];
    } else {
        frame = animationFrames[currentIdleFrame];
    }

    Rectangle source = {
        0.0f, 0.0f,
        static_cast<float>(frame.width) * (facingLeft ? -1.0f : 1.0f),
        static_cast<float>(frame.height)
    };

    Rectangle dest = {
        position.x,
        position.y,
        TOWER_SIZE,
        TOWER_SIZE
    };

    Vector2 origin = {
        TOWER_SIZE / 2.0f,
        TOWER_SIZE / 2.0f
    };

    DrawTexturePro(frame, source, dest, origin, rotationAngle, WHITE);
}

void Torcher::showUpgradeInfo() const {
    
    int infoX = GetScreenWidth() / 2 + 270;
    int infoY = GetScreenHeight() - 160;

    int name_font_size = 20;
    int stat_font_size = 18;

    switch (level) {
        case 1:
            DrawText("Sharp Sight", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Damage", infoX + 10, infoY + 40, stat_font_size, WHITE);
            DrawText("+ Range", infoX + 10, infoY + 70, stat_font_size, WHITE);
            break;
        case 2:
            DrawText("Pyric Conduit", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Damage", infoX + 10, infoY + 40, stat_font_size, WHITE);
            DrawText("+ Attack Speed", infoX + 10, infoY + 70, stat_font_size, WHITE);
            DrawText("NEW: Flame Slows", infoX + 10, infoY + 100, stat_font_size, WHITE);
            break;
        case 3:
            DrawText("Increased Fuel", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Burn Duration", infoX + 10, infoY + 40, stat_font_size, WHITE);
            DrawText("+ Burn Ticks", infoX + 10, infoY + 70, stat_font_size, WHITE);
            break;
        case 4:
            DrawText("Flamecaller", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ ALL stats", infoX + 10, infoY + 40, stat_font_size,  WHITE);
            break;
        case 5:
            DrawText("Maxed", infoX + 10, infoY + 10, name_font_size, WHITE);
            break;
    }
}

Stormcaller::Stormcaller(Vector2 pos) : Tower(300, 30, 0.2f, "Area of Effect", 3000, pos) {
    name = "Stormcaller";
    type = "Air";
    value = cost / 2;
    projectileSpeed = 4000.0f;
    projectileRange = range * 2;
    AoERadius = 10.0f;
    shootFlashDuration = 5.0f;
    animationFrames = ImageHandler::LoadAnimationFrames("stormcaller", 50);
}

void Stormcaller::update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track, std::vector<std::shared_ptr<Projectile>>& projectiles) {

    attackCooldown -= deltaTime;
    shootTimer -= deltaTime; // For shoot vs. idle animation

    isShooting = shootTimer > 0.0f;

    if (isShooting) {
        shootingFrameTimer += deltaTime;
        if (shootingFrameTimer >= shootingFrameDuration) {
            shootingFrameTimer -= shootingFrameDuration;
            shootingIndex = (shootingIndex + 1) % 6;
            currentShootingFrame = frameIndex + shootingIndex;
            //currentShootingFrame = (currentShootingFrame + 1) % 6;
        }
    } else {
        shootingFrameTimer += deltaTime;
        if (shootingFrameTimer >= shootingFrameDuration) {
            shootingFrameTimer -= shootingFrameDuration;
            idleIndex = (idleIndex + 1) % 4;
            currentIdleFrame = frameIndex + 6 + idleIndex;
            //currentIdleFrame = ((currentIdleFrame + 1) % 4) + 6;
        }
    }

    if (attackCooldown > 0) return;

    auto target = EnemyToShoot(mode, enemies, track);

    if (target) {
        Vector2 toTarget = Vector2Subtract(target->getPosition(), position);
        float angleRad = atan2f(toTarget.y, toTarget.x);
        float angleDeg = angleRad * RAD2DEG;

        isShooting = true;
        facingLeft = (toTarget.x < 0);
        rotationAngle = facingLeft ? angleDeg + 180.0f : angleDeg;

        shootTimer = shootFlashDuration;

        PlaySound(SoundManager::thunder);
        DrawLightningBolt(target);

        int actualDamage = static_cast<int>(std::ceil(damage * damageMultiplier));
        float actualAttackSpeed = attackSpeed * attackSpeedMultiplier;

        ApplyAOEDamage(target->getPosition(), AoERadius, actualDamage, type);
        explosions.push_back({target->getPosition(), AoERadius});
        attackCooldown = 1.0f / actualAttackSpeed;
    }
}

void Stormcaller::upgrade(int upgCost) {
    level += 1;
    if (level > 5) {
        level = 5;
        return;
    } else {
        value += (upgCost / 2);
    }

    frameIndex += 10;

    switch (level) {
        case 2:
            attackSpeed = 0.4;
            break;
        case 3:
            damage += 20;
            range += 50;
            break;
        case 4:
            damage += 30;
            attackSpeed = 0.5;
            AoERadius += 5;
            break;
        case 5:
            damage += 100;
            range += 50;
            attackSpeed = 0.75;
            AoERadius += 5;
            break;
    }
}

void Stormcaller::DrawLightningBolt(std::shared_ptr<Enemy> target, int segments, float offset, Color color) {
    // Create lightning bolt
    LightningBolt bolt;
    bolt.start = position;
    bolt.end = target->getPosition();

    if (level <= 3) {
        bolt.color = SKYBLUE;
    } else if (level == 4) {
        bolt.color = PURPLE;
    } else {
        bolt.color = YELLOW;
    }

    // Generate bolt shape
    Vector2 lastPoint = bolt.start;

    for (int i = 1; i <= segments; i++) {
        float t = (float)i / (float)segments;
        Vector2 point = {
            bolt.start.x + (bolt.end.x - bolt.start.x) * t,
            bolt.start.y + (bolt.end.y - bolt.start.y) * t
        };
        point.x += GetRandomValue(-offset, offset);
        point.y += GetRandomValue(-offset, offset);
        bolt.points.push_back(point);
    }

    lightningBolts.push_back(bolt);
}

void Stormcaller::draw() const {
    Texture2D frame;

    if (isShooting) {
        frame = animationFrames[currentShootingFrame];
    } else {
        frame = animationFrames[currentIdleFrame];
    }

    Rectangle source = {
        0.0f, 0.0f,
        static_cast<float>(frame.width) * (facingLeft ? -1.0f : 1.0f),
        static_cast<float>(frame.height)
    };

    Rectangle dest = {
        position.x,
        position.y,
        TOWER_SIZE,
        TOWER_SIZE
    };

    Vector2 origin = {
        TOWER_SIZE / 2.0f,
        TOWER_SIZE / 2.0f
    };

    DrawTexturePro(frame, source, dest, origin, rotationAngle, WHITE);
}

void Stormcaller::showUpgradeInfo() const {
    
    int infoX = GetScreenWidth() / 2 + 270;
    int infoY = GetScreenHeight() - 160;

    int name_font_size = 20;
    int stat_font_size = 18;

    switch (level) {
        case 1:
            DrawText("Faster Channeling", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Attack Speed", infoX + 10, infoY + 40, stat_font_size, WHITE);
            break;
        case 2:
            DrawText("Stormpulse", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Damage", infoX + 10, infoY + 40, stat_font_size, WHITE);
            DrawText("+ Range", infoX + 10, infoY + 70, stat_font_size, WHITE);
            break;
        case 3:
            DrawText("Thundercrash", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Damage", infoX + 10, infoY + 40, stat_font_size, WHITE);
            DrawText("+ Attack Speed", infoX + 10, infoY + 70, stat_font_size, WHITE);
            DrawText("+ AoE Radius", infoX + 10, infoY + 100, stat_font_size, WHITE);
            break;
        case 4:
            DrawText("Eye of the Storm", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ ALL stats", infoX + 10, infoY + 40, stat_font_size,  WHITE);
            break;
        case 5:
            DrawText("Maxed", infoX + 10, infoY + 10, name_font_size, WHITE);
            break;
    }
}

War_Drummer::War_Drummer(Vector2 pos) : Tower(200, 0, 0.0f, "Towers", 1000, pos) {
    name = "War Drummer";
    type = "None";
    value = cost / 2;
    damageMultiplier = 1.15f;
    attackSpeedMultiplier = 1.2f;
    shootFlashDuration = 4.0f;
    animationFrames = ImageHandler::LoadAnimationFrames("wardrummer", 4);
}

void War_Drummer::update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track, std::vector<std::shared_ptr<Projectile>>& projectiles) {

    shootTimer += deltaTime; // War drummer doesn't shoot, it's just for frame timers

    if (shootTimer >= shootFlashDuration) {
        currentFrame = GetRandomValue(0, 3);
        shootTimer = 0.0f;
    }

    for (auto& tower : towers) {
        if (tower.get() == this || tower->getName() == "War Drummer") continue; // Don't buff self or other War Drummers
        float dist = Vector2Distance(this->getPosition(), tower->getPosition());
        if (dist <= this->getRange()) {
            float currentTowerDmgMult = tower->getDamageMultiplier();
            float currentTowerAtkSpdMult = tower->getAttackSpeedMultiplier();
            if (!tower->getWarDrummerBuff()) {
                tower->setWarDrummerBuff();
                tower->setDamageMultiplier(damageMultiplier);
                tower->setAttackSpeedMultiplier(attackSpeedMultiplier);
                continue;
            } 
            
            // Should change this code if in the future create more towers that buff these two stats
            // But for now, this should work with only War Drummer as the stat boosting tower
            if (currentTowerDmgMult < damageMultiplier) {
                tower->setDamageMultiplier(damageMultiplier);
            }

            if (currentTowerAtkSpdMult < attackSpeedMultiplier) {
                tower->setAttackSpeedMultiplier(attackSpeedMultiplier);
            }
        }
    }
}

void War_Drummer::upgrade(int upgCost) {
    level += 1;
    if (level > 5) {
        level = 5;
        return;
    } else {
        value += (upgCost / 2);
    }
    switch (level) {
        case 2:
            range += 50;
            shootFlashDuration = 2.0f;
            break;
        case 3:
            damageMultiplier += 0.05f;
            shootFlashDuration = 1.0f;
            break;
        case 4:
            attackSpeedMultiplier += 0.1f;
            shootFlashDuration = 0.5f;
            break;
        case 5:
            range += 50;
            damageMultiplier += 0.1f;
            attackSpeedMultiplier += 0.1f;
            shootFlashDuration = 0.25f;
            break;
    }
}

void War_Drummer::draw() const {

    Texture2D frame = animationFrames[currentFrame];

    Rectangle source = {
        0.0f, 0.0f,
        static_cast<float>(frame.width),
        static_cast<float>(frame.height)
    };

    Rectangle dest = {
        position.x,
        position.y,
        TOWER_SIZE,
        TOWER_SIZE
    };

    Vector2 origin = {
        TOWER_SIZE / 2,
        TOWER_SIZE / 2
    };

    DrawTexturePro(frame, source, dest, origin, 0.0f, WHITE);
}

void War_Drummer::showUpgradeInfo() const {
    int infoX = GetScreenWidth() / 2 + 270;
    int infoY = GetScreenHeight() - 160;

    int name_font_size = 20;
    int stat_font_size = 18;

    switch (level) {
        case 1:
            DrawText("Bigger Drums", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Range", infoX + 10, infoY + 40, stat_font_size, WHITE);
            break;
        case 2:
            DrawText("Drum of Might", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Damage Multiplier", infoX + 10, infoY + 40, stat_font_size, WHITE);
            break;
        case 3:
            DrawText("Drum of Fury", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Attack Speed Multiplier", infoX + 10, infoY + 40, 16, WHITE);
            break;
        case 4:
            DrawText("Eternal War Drum", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ ALL stats", infoX + 10, infoY + 40, stat_font_size,  WHITE);
            break;
        case 5:
            DrawText("Maxed", infoX + 10, infoY + 10, name_font_size, WHITE);
            break;
    }
}

Gold_Mine::Gold_Mine(Vector2 pos) : Tower(50, 0, 0.0f, "Utility", 300, pos) {
    name = "Gold Mine";
    type = "None";
    value = cost / 2;
    animationFrames = ImageHandler::LoadAnimationFrames("goldmine", 5);
}

int Gold_Mine::getTotalMoneyGenerated() const {
    return totalGoldGenerated;
}

void Gold_Mine::generate(int& playerMoney) {
    PlaySound(SoundManager::money);
    playerMoney += goldPerRound;
    totalGoldGenerated += goldPerRound;

    FloatingText text;
    text.position = getPosition();
    text.text = "+ " + std::to_string(goldPerRound);
    floatingTexts.push_back(text);
}

void Gold_Mine::update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track, std::vector<std::shared_ptr<Projectile>>& projectiles) {
    return;
}

void Gold_Mine::upgrade(int upgCost) {
    level += 1;
    if (level > 5) {
        level = 5;
        return;
    } else {
        value += (upgCost / 2);
    }

    frame += 1;

    switch (level) {
        case 2:
            goldPerRound = 200;
            break;
        case 3:
            goldPerRound = 500;
            break;
        case 4:
            goldPerRound = 1000;
            break;
        case 5:
            goldPerRound = 2000;
            break;
    }
}

void Gold_Mine::draw() const {
    Vector2 center = getPosition();  // center of the tower
    Texture2D icon = animationFrames[frame];

    float desiredSize = 40.0f;
    float scale = desiredSize / icon.width;

    float iconWidth = icon.width * scale;
    float iconHeight = icon.height * scale;

    Vector2 topLeft = {
        center.x - iconWidth / 2.0f,
        center.y - iconHeight / 2.0f
    };

    DrawTextureEx(icon, topLeft, 0.0f, scale, WHITE);
}

void Gold_Mine::showUpgradeInfo() const {

    int infoX = GetScreenWidth() / 2 + 270;
    int infoY = GetScreenHeight() - 160;

    int name_font_size = 20;
    int stat_font_size = 18;

    switch (level) {
        case 1:
            DrawText("Refined Shaft", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Income Per Round", infoX + 10, infoY + 40, stat_font_size, WHITE);
            break;
        case 2:
            DrawText("Bigger Carts", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Income Per Round", infoX + 10, infoY + 40, stat_font_size, WHITE);
            break;
        case 3:
            DrawText("Reinforced Tunnels", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Income Per Round", infoX + 10, infoY + 40, stat_font_size, WHITE);
            break;
        case 4:
            DrawText("Automated Drills", infoX + 10, infoY + 10, name_font_size, WHITE);
            DrawText("+ Income Per Round", infoX + 10, infoY + 40, stat_font_size, WHITE);
            break;
        case 5:
            DrawText("Maxed", infoX + 10, infoY + 10, name_font_size, WHITE);
            break;
    }
}