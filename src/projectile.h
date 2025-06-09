#pragma once
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <memory>
#include <unordered_set>

class Enemy;

class Tower;

class Projectile {
    private:
        Vector2 position;
        Vector2 direction;
        float speed;
        int damage;
        int pierceCount = 1;
        float AoERadius = 0.0f;
        std::unordered_set<Enemy *> hitEnemies;
        bool active = true;
        std::string type;
        std::weak_ptr<Tower> sourceTower;

    public:
        Projectile(Vector2 pos, Vector2 dir, float spd, int dmg, std::string type, std::weak_ptr<Tower> source, int pierceCount, float AoERadius);

        void update(float deltaTime, std::weak_ptr<Tower> source);

        void draw() const;

        void drawExplosion() const;

        bool isActive() const;
        void deactivate();

        Vector2 getPosition() const;
        int getDamage() const;
        std::string getDamageType() const;

        std::weak_ptr<Tower> getSourceTower() const;

        void setPierceCount(int count);
        int getPierceCount() const;

        void setAOERadius(float radius);
        float getAOERadius() const;

        bool hasHit(Enemy* enemy) const;
        void markHit(Enemy* enemy);
        
};
