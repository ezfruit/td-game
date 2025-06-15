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
    public:
        virtual void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::shared_ptr<Projectile>>& projectiles) = 0;

        virtual void upgrade(int upgCost) = 0;

        bool canShoot() const;

        Vector2 getPosition() const;

        std::string getName() const;

        int getRange() const;

        int getDamage() const;

        void setDamageMultiplier(float multiplier);

        void setAttackSpeedMultiplier(float multiplier);

        void setWarDrummerBuff();

        std::string getTargeting() const;

        void setTotalDamageDealt(int dmg);

        int getTotalDamageDealt() const;

        void setValue(int val);

        int getValue() const;

        int getLevel() const;

        void resetBuffs();

        bool IsInRange(std::shared_ptr<Enemy> enemy);

        std::shared_ptr<Enemy> FindStrongestTarget(std::vector<std::shared_ptr<Enemy>>& enemies);

        void ApplyAOEDamage(Vector2 center, float radius, int damage, std::string type);

        float getProjectileRange() const;

        virtual ~Tower() = default;

        Tower(int range, int damage, float attack_speed, std::string targeting, int cost, Vector2 pos);
};

class Archer : public Tower {
    public:

        void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::shared_ptr<Projectile>>& projectiles) override;

        void upgrade(int upgCost) override;

        Archer(Vector2 pos);
};

class Mage : public Tower {
    public:
        void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::shared_ptr<Projectile>>& projectiles) override;

        void upgrade(int upgCost) override;

        Mage(Vector2 pos);
};

class Torcher : public Tower {
    private:

        float fireCooldown = 1.0f;
        float timeSinceLastFire = 0.0f;

        float burnDelay = 1.0f;
        float burnDuration = 5.0f;

        float slowEffect = 1.0f;

        std::shared_ptr<Enemy> FindUnburnedTarget(std::vector<std::shared_ptr<Enemy>>& enemies);
    public:
        void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::shared_ptr<Projectile>>& projectiles) override;

        void upgrade(int upgCost) override;

        void FireAt(std::shared_ptr<Enemy> enemy, int actualDamage);

        Torcher(Vector2 pos);
};

class Stormshaper : public Tower {
    public:
        void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::shared_ptr<Projectile>>& projectiles) override;

        void upgrade(int upgCost) override;

        void DrawLightningBolt(std::shared_ptr<Enemy> target, int segments = 10, float offset = 10.0f, Color color = SKYBLUE);

        Stormshaper(Vector2 pos);
};

class War_Drummer : public Tower {
    public:
        void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::shared_ptr<Projectile>>& projectiles) override;

        void upgrade(int upgCost) override;

        War_Drummer(Vector2 pos);
};

class Gold_Mine : public Tower {
    private:
        int totalGoldGenerated = 0;
        int goldPerRound = 100;
    public:

        int getTotalMoneyGenerated() const;

        void generate(int& playerMoney);

        void update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::shared_ptr<Projectile>>& projectiles) override;

        void upgrade(int upgCost) override;

        Gold_Mine(Vector2 pos);
};