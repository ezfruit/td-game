#pragma once
#include "raylib.h"
#include <iostream>
#include <vector>
#include "enemy.h"

class Tower {
    protected:
        Vector2 position;
        int range;
        int damage;
        double attack_speed;
        std::string targeting;
        int cost;
        int upgrade_level = 0;
    public:
        virtual void attack(std::vector<Enemy*>& enemies) = 0;
        virtual void upgrade() = 0;

        Vector2 getPosition() const;

        virtual ~Tower() = default;

        Tower(int range, int damage, double attack_speed, std::string targeting, int cost, Vector2 pos);
};

class Archer : public Tower {
    public:

        void attack(std::vector<Enemy*>& enemies) override;

        void upgrade() override;

        Archer(Vector2 pos);
};

class Mage : public Tower {
    public:
        void attack(std::vector<Enemy*>& enemies) override;

        void upgrade() override;

        Mage(Vector2 pos);
};