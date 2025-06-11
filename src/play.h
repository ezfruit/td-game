#pragma once
#include "raymath.h"
#include <vector>
#include <memory>

extern std::vector<Vector2> trackPoints;

class Enemy;
class Projectile;

extern std::vector<std::shared_ptr<Enemy>> enemies;
extern std::vector<std::shared_ptr<Projectile>> projectiles;

extern bool GameOver;
extern bool Paused;
extern bool HomePressed;

extern int playerMoney;

void InitPlaying();

void ResetGame();

void UpdatePlaying();

void DrawPlaying();