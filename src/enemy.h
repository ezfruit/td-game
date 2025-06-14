#pragma once
#include <iostream>
#include <vector>

class Tower;

class Enemy {
    protected:
        Vector2 position;
        std::string name;
        int health;
        int maxHealth;
        float hitboxRadius;
        float speed;
        float baseSpeed;
        int currentTarget = 0;
        bool burning = false;
        float burnStartTime = 0.0f;
        float burnEndTime;
        float burnDuration = 0.0f;
        float burnDamage;
        float burnDelay;
        float nextBurnTick;
        std::weak_ptr<Tower> burnSource;
        bool alive = true;
    public:
        virtual void takeDamage(int amount, const std::string& type) = 0;

        virtual std::string getName() const = 0;

        void reduceSpeed(float spd);

        void revertSpeed();

        void setPosition(Vector2 pos);

        virtual void update(float deltaTime, const std::vector<Vector2>& track);

        Vector2 getPosition() const;

        int getHealth() const;

        int getMaxHealth() const;

        float getRadius() const;
        
        bool isAlive() const;

        bool isBurning() const;

        void setCurrentTarget(int target);

        void applyBurn(float delay, float dps, float duration, float slowEffect, std::weak_ptr<Tower> source);

        virtual ~Enemy() = default;

        Enemy(int health, float speed, float radius);
};

class Slime : public Enemy {
    public:
        std::string getName() const override;

        void takeDamage(int amount, const std::string& type) override;

        Slime();
};

class Knight : public Enemy {
    public:
        std::string getName() const override;

        void takeDamage(int amount, const std::string& type) override;

        Knight();
};

class Fire_Imp : public Enemy {
    public:
        std::string getName() const override;

        void takeDamage(int amount, const std::string& type) override;

        Fire_Imp();
};

class Brute : public Enemy {
    public:
        std::string getName() const override;

        void takeDamage(int amount, const std::string& type) override;

        Brute();
};

class SpawnableEnemy : public Enemy {
    protected:
        float spawnCooldownTimer = 0.0f;
        float spawnDelayTimer = 0.0f;
        float spawnDelay; // delay between each enemy spawn
        float spawnCooldown; // seconds between spawns
        int spawnCount;
        int remainingSpawns;
        bool inCooldown = false;
    public:

        void update(float deltaTime, const std::vector<Vector2>& track) override;

        virtual void spawn() = 0;

        SpawnableEnemy(float hp, float speed, float radius, float delay, float cooldown, int spawnCount);
};


class Spider_Queen : public SpawnableEnemy {
    public:
        std::string getName() const override;

        void takeDamage(int amount, const std::string& type) override;

        void spawn() override;

        Spider_Queen();
};

class Spiderling : public Enemy {
    public:

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type) override;

        Spiderling();
};

class Arcane_Shell : public Enemy {
    public:

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type) override;

        Arcane_Shell();
};

class Flux : public Enemy {
    private:
        float shieldChangeTimer = 0.0f;
        float shieldChangeCooldown = 5.0f;
        std::string shield = "Physical";
    public:

        void update(float deltaTime, const std::vector<Vector2>& track);

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type) override;

        void setShield(std::string shield);

        std::string getShield() const;

        Flux();
};

class Husk : public Enemy {
    public:
        std::string getName() const override;

        void takeDamage(int amount, const std::string& type) override;

        Husk();
};