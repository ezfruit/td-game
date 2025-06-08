#include "raylib.h"
#include "raymath.h"
#include "projectile.h"

Projectile::Projectile(Vector2 pos, Vector2 dir, float spd, int dmg) : position(pos), direction(Vector2Normalize(dir)), speed(spd), damage(dmg) {}

void Projectile::update(float deltaTime) {
    position = Vector2Add(position, Vector2Scale(direction, speed * deltaTime));
}

void Projectile::draw() const {
    DrawCircleV(position, 5, RED);
}