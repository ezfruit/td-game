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
        float damageMultiplier = 1.0f;
        int totalDamageDealt = 0;
        float attackCooldown = 0.0f;
        float attackSpeed;
        float attackSpeedMultiplier = 1.0f;
        bool war_drummer_buff = false;
        float projectileSpeed;
        float projectileRange;
        int pierceCount = 1;
        float AoERadius = 0.0f;
        std::string targeting;
        int cost;
        int value;
        int level = 1;
        TargetMode mode = FIRST;
        std::vector<Texture2D> animationFrames;
        float rotationAngle = 0.0f; // in degrees
        float shootFlashDuration = 2.0f;
        float shootTimer = 0.0f;
        bool isShooting = false;
        bool facingLeft = false;
    public:
        Tower(int range, int damage, float attack_speed, std::string targeting, int cost, Vector2 pos);

        virtual ~Tower() = default;

        virtual void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track, std::vector<std::shared_ptr<Projectile>>& projectiles) = 0;

        virtual void upgrade(int upgCost) = 0;

        virtual void draw() const = 0;

        bool canShoot() const;

        Vector2 getPosition() const;

        std::string getName() const;

        int getRange() const;

        int getDamage() const;

        void setDamageMultiplier(float multiplier);

        void setAttackSpeedMultiplier(float multiplier);

        void setWarDrummerBuff();

        bool getWarDrummerBuff() const;

        std::string getTargeting() const;

        void setTotalDamageDealt(int dmg);

        int getTotalDamageDealt() const;

        void setValue(int val);

        int getValue() const;

        int getLevel() const;

        void resetBuffs();

        bool IsInRange(std::shared_ptr<Enemy> enemy);

        std::shared_ptr<Enemy> FindFirstTarget(std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track);

        std::shared_ptr<Enemy> FindLastTarget(std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track);

        std::shared_ptr<Enemy> FindStrongestTarget(std::vector<std::shared_ptr<Enemy>>& enemies);

        std::shared_ptr<Enemy> FindWeakestTarget(std::vector<std::shared_ptr<Enemy>>& enemies);

        void ApplyAOEDamage(Vector2 center, float radius, int damage, std::string type);

        float getProjectileRange() const;

        std::shared_ptr<Enemy> EnemyToShoot(TargetMode mode, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track);

        void setTargetMode(TargetMode newMode);

        TargetMode getTargetMode() const;

        void unloadFrames();
};

class Archer : public Tower {
    public:
        Archer(Vector2 pos);

        void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track, std::vector<std::shared_ptr<Projectile>>& projectiles) override;

        void upgrade(int upgCost) override;

        void draw() const override;
};

class Mage : public Tower {
    public:
        Mage(Vector2 pos);

        void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track, std::vector<std::shared_ptr<Projectile>>& projectiles) override;

        void upgrade(int upgCost) override;

        void draw() const override;
};

class Torcher : public Tower {
    private:
        float burnDelay = 1.0f;
        float burnDuration = 5.0f;

        float slowEffect = 1.0f;

        float shootingFrameTimer = 0.0f;       
        int currentShootingFrame = 0;
        float shootingFrameDuration = 0.1f; // 0.1s = 10 FPS

        std::shared_ptr<Enemy> FindUnburnedTarget(std::vector<std::shared_ptr<Enemy>>& enemies);
    public:
        Torcher(Vector2 pos);

        void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track, std::vector<std::shared_ptr<Projectile>>& projectiles) override;

        void upgrade(int upgCost) override;

        void FireAt(std::shared_ptr<Enemy> enemy, int actualDamage);

        void draw() const override;
};

class Stormcaller : public Tower {
    private:
        float shootingFrameTimer = 0.0f;
        float idleFrameTimer = 0.0f;       
        int currentShootingFrame = 0;
        int currentIdleFrame = 0;
        float shootingFrameDuration = 0.1f; // 0.1s = 10 FPS

    public:
        Stormcaller(Vector2 pos);

        void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track, std::vector<std::shared_ptr<Projectile>>& projectiles) override;

        void upgrade(int upgCost) override;

        void DrawLightningBolt(std::shared_ptr<Enemy> target, int segments = 10, float offset = 10.0f, Color color = SKYBLUE);

        void draw() const override;
};

class War_Drummer : public Tower {
    public:
        War_Drummer(Vector2 pos);

        void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track, std::vector<std::shared_ptr<Projectile>>& projectiles) override;

        void upgrade(int upgCost) override;

        void draw() const override;
};

class Gold_Mine : public Tower {
    private:
        int totalGoldGenerated = 0;
        int goldPerRound = 100;
    public:
        Gold_Mine(Vector2 pos);

        int getTotalMoneyGenerated() const;

        void generate(int& playerMoney);

        void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<Vector2>& track, std::vector<std::shared_ptr<Projectile>>& projectiles) override;

        void upgrade(int upgCost) override;

        void draw() const override;
};