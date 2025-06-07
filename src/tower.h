#pragma once
#include "raylib.h"
#include "enemy.h"

class Tower {
    protected:
        Vector2 position;
        std::string name;
        int range;
        int damage;
        int totalDamageDealt = 0;
        double attack_speed;
        std::string targeting;
        int cost;
        int value;
        int level = 1;
    public:
        virtual void attack(std::vector<Enemy*>& enemies) = 0;

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

        Tower(int range, int damage, double attack_speed, std::string targeting, int cost, Vector2 pos);
};

class Archer : public Tower {
    public:

        void attack(std::vector<Enemy*>& enemies) override;

        void upgrade(int upgCost) override;

        Archer(Vector2 pos);
};

class Mage : public Tower {
    public:
        void attack(std::vector<Enemy*>& enemies) override;

        void upgrade(int upgCost) override;

        Mage(Vector2 pos);
};