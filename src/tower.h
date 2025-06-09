#pragma once
#include "raylib.h"
#include "raymath.h"
#include "enemy.h"
#include "play.h"
#include <memory>
#include "projectile.h"

class Tower : public std::enable_shared_from_this<Tower> {
    protected:
        Vector2 position;
        std::string name;
        std::string type;
        int range;
        int damage;
        int totalDamageDealt = 0;
        float attackCooldown = 0.0f;
        float attackSpeed;
        float projectileSpeed;
        float projectileRange;
        int pierceCount = 1;
        float AoERadius = 0.0f;
        std::string targeting;
        int cost;
        int value;
        int level = 1;
    public:
        virtual void attack(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles) = 0;

        virtual void upgrade(int upgCost) = 0;

        bool canShoot() const;

        Vector2 getPosition() const;

        std::string getName() const;

        int getRange() const;

        int getDamage() const;

        void setTotalDamageDealt(int dmg);

        int getTotalDamageDealt() const;

        void setValue(int val);

        int getValue() const;

        int getLevel() const;

        float getProjectileRange() const;

        virtual ~Tower() = default;

        Tower(int range, int damage, float attack_speed, std::string targeting, int cost, Vector2 pos);
};

class Archer : public Tower {
    public:

        void attack(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles) override;

        void upgrade(int upgCost) override;

        Archer(Vector2 pos);
};

class Mage : public Tower {
    public:
        void attack(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles) override;

        void upgrade(int upgCost) override;

        Mage(Vector2 pos);
};