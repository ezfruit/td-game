#pragma once
#include "raymath.h"
#include <vector>
#include <memory>
#include "explosion.h"

extern std::vector<Vector2> trackPoints;

class Enemy;
class Projectile;

extern std::vector<std::shared_ptr<Enemy>> enemies;
extern std::vector<std::shared_ptr<Projectile>> projectiles;
extern std::vector<Explosion> explosions;
extern std::vector<LightningBolt> lightningBolts;

extern bool GameOver;
extern bool Paused;
extern bool HomePressed;

extern int playerMoney;

void InitPlaying();

void ResetGame();

void UpdatePlaying();

void DrawPlaying();