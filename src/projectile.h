#pragma once
#include "raylib.h"
#include "raymath.h"

class Projectile {
    public:
        Vector2 position;
        Vector2 direction;
        float speed;
        int damage;
        bool active = true;

        Projectile(Vector2 pos, Vector2 dir, float spd, int dmg);

        void update(float deltaTime);

        void draw() const;
};
