#pragma once
#include <iostream>
#include <vector>
#include "images.h"

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
        int currentFrame = 0;
        float animationTimer = 0.0f;
        float frameSpeed = 0.125f;
        std::vector<Texture2D> moveFrames;
    public:
        Enemy(int health, float speed, float radius);

        virtual ~Enemy() = default;

        virtual void takeDamage(int amount, const std::string& type, const std::string& targeting) = 0;

        virtual std::string getName() const = 0;

        void reduceSpeed(float spd);

        void revertSpeed();

        void setPosition(Vector2 pos);

        float getPathProgress(const std::vector<Vector2>& track) const;

        virtual void update(float deltaTime, const std::vector<Vector2>& track);

        Vector2 getPosition() const;

        int getHealth() const;

        int getMaxHealth() const;

        float getRadius() const;
        
        bool isAlive() const;

        bool isBurning() const;

        void setCurrentTarget(int target);

        void applyBurn(float delay, float dps, float duration, float slowEffect, std::weak_ptr<Tower> source);

        void unloadFrames();

        virtual void draw() const = 0;
};

extern std::vector<std::shared_ptr<Enemy>> deathSpawns;

class Slime : public Enemy {
    public:
        Slime();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void draw() const override;
};

class Knight : public Enemy {
    public:
        Knight();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void draw() const override;
};

class Fire_Imp : public Enemy {
    public:
        Fire_Imp();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void draw() const override;
};

class Brute : public Enemy {
    public:
        Brute();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void draw() const override;
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
        SpawnableEnemy(float hp, float speed, float radius, float delay, float cooldown, int spawnCount);

        void update(float deltaTime, const std::vector<Vector2>& track) override;

        virtual void spawn() = 0;
};


class Spider_Queen : public SpawnableEnemy {
    public:
        Spider_Queen();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void spawn() override;

        void draw() const override;
};

class Spiderling : public Enemy {
    public:
        Spiderling();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void draw() const override;
};

class Arcane_Shell : public Enemy {
    public:
        Arcane_Shell();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void draw() const override;
};

class Flux : public Enemy {
    private:
        float shieldChangeTimer = 0.0f;
        float shieldChangeCooldown = 5.0f;
        std::string shield = "Physical";
    public:
        Flux();

        void update(float deltaTime, const std::vector<Vector2>& track);

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void draw() const override;

        void setShield(std::string shield);

        std::string getShield() const;
};

class Husk : public Enemy {
    public:
        Husk();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void draw() const override;
};

class Exoskeleton : public Enemy {
    public:
        Exoskeleton();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void draw() const override;

};

class Goliath : public Enemy {
    public:
        Goliath();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void draw() const override;
};

class Sludge : public Enemy {
    public:
        Sludge();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void spawn();

        void draw() const override;
};

class Sludge_Mite : public Enemy {
    public:
        Sludge_Mite();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void draw() const override;
};

class Lava_Golem : public Enemy {
    public:
        Lava_Golem();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void draw() const override;
};

class Obsidian_Behemoth : public Enemy {
    public:
        Obsidian_Behemoth();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void draw() const override;
};

class Ravager : public Enemy {
    public:
        Ravager();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void draw() const override;
};

class Arcane_Warden : public SpawnableEnemy {
    public:
        Arcane_Warden();

        std::string getName() const override;

        void takeDamage(int amount, const std::string& type, const std::string& targeting) override;

        void spawn() override;

        void draw() const override;
};