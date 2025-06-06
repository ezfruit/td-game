#pragma once
#include <iostream>
#include <vector>

class Enemy {
    protected:
        Vector2 position;
        std::string name;
        int health;
        float speed;
        float baseSpeed;
        int currentTarget = 0;
        bool alive = true;
    public:
        virtual void takeDamage(int amount, const std::string& type) = 0;

        virtual std::string getName() const = 0;

        void reduceSpeed(float spd);

        void revertSpeed();

        void setPosition(Vector2 pos);

        void update(float deltaTime, const std::vector<Vector2>& track);

        Vector2 getPosition() const;

        int getHealth() const;
        
        bool isAlive() const;

        virtual ~Enemy() = default;

        Enemy(int health, float speed);
};

class Slime : public Enemy {
    public:
        std::string getName() const override;

        void takeDamage(int amount, const std::string& type) override;

        Slime();
};

class Armored_Knight : public Enemy {
    public:
        std::string getName() const override;

        void takeDamage(int amount, const std::string& type) override;

        Armored_Knight();
};