#pragma once
#include "raylib.h"

struct Explosion {
    Vector2 position;
    float radius;
    float duration = 0.4f;
    float timeAlive = 0.0f;
    bool active = true;
};

struct LightningBolt {
    Vector2 start;
    Vector2 end;
    float duration = 2.0f; // lasts 2 seconds
    float timeAlive = 0.0f;
    std::vector<Vector2> points; // precomputed zigzag path
    Color color;
    bool active = true;
};