#pragma once
#include "raylib.h"
#include "raymath.h"
#include "enemy.h"
#include "play.h"

class Tower {
    protected:
        Vector2 position;
        std::string name;
        std::string type;
        int range;
        int damage;
        int totalDamageDealt = 0;
        float attackCooldown = 0.0f;
        float attackSpeed;
        std::string targeting;
        int cost;
        int value;
        int level = 1;
    public:
        virtual void attack(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) = 0;

        virtual void upgrade(int upgCost) = 0;

        Vector2 getPosition() const;

        std::string getName() const;

        int getRange() const;

        int getDamage() const;

        void setTotalDamageDealt(int dmg);

        int getTotalDamageDealt() const;

        void setValue(int val);

        int getValue() const;

        int getLevel() const;

        virtual ~Tower() = default;

        Tower(int range, int damage, float attack_speed, std::string targeting, int cost, Vector2 pos);
};

class Archer : public Tower {
    public:

        void attack(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) override;

        void upgrade(int upgCost) override;

        Archer(Vector2 pos);
};

class Mage : public Tower {
    public:
        void attack(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) override;

        void upgrade(int upgCost) override;

        Mage(Vector2 pos);
};