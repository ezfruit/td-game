#pragma once
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <memory>
#include <unordered_set>

class Enemy;

class Tower;

class Projectile {
    protected:
        Vector2 position;
        Vector2 direction;
        float speed;
        int damage;
        int pierceCount = 1;
        float AoERadius = 0.0f;
        std::unordered_set<Enemy *> hitEnemies;
        bool active = true;
        std::string type; // Physical, Magic, Fire, Air, etc.
        std::string targeting; // AoE, Single, Pierce, etc.
        std::weak_ptr<Tower> sourceTower;

    public:
        Projectile(Vector2 pos, Vector2 dir, float spd, int dmg, std::string type, std::string targeting, std::weak_ptr<Tower> source, int pierceCount, float AoERadius);

        virtual void update(float deltaTime, std::weak_ptr<Tower> source);

        void draw() const;

        void drawExplosion() const;

        bool isActive() const;
        void deactivate();

        Vector2 getPosition() const;
        int getDamage() const;
        std::string getDamageType() const;
        std::string getDamageTargeting() const;

        std::weak_ptr<Tower> getSourceTower() const;

        void setPierceCount(int count);
        int getPierceCount() const;

        void setAOERadius(float radius);
        float getAOERadius() const;

        void ApplyAOEDamage(Projectile& projectile, Vector2 center, float radius, int damage, std::string type);

        bool hasHit(Enemy* enemy) const;
        void markHit(Enemy* enemy);
        
};

class Flames : public Projectile {
    private:
        std::shared_ptr<Enemy> target;
        float burnDelay, burnDPS, burnDuration, slowEffect;
        float burnTimer = 0.0f;
        bool hasAppliedBurn = false;
        bool hasReachedTarget() const;
    public:

        void setTarget(std::shared_ptr<Enemy> enemy);

        void setBurnDelay(float delay);

        void setBurnDamage(int dmg);

        void setBurnDuration(float duration);

        void setSlowEffect(float slow);

        void update(float deltaTime, std::weak_ptr<Tower> source);
        
        Flames(Vector2 pos, Vector2 dir, float spd, int dmg, std::string type, std::string targeting, std::weak_ptr<Tower> source, int pierceCount, float AoERadius);
};
