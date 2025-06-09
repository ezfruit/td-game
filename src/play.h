#pragma once
#include "raymath.h"
#include <vector>
#include <memory>

extern std::vector<Vector2> trackPoints;

class Enemy;

extern std::vector<std::shared_ptr<Enemy>> enemies;

extern bool GameOver;
extern bool Paused;

extern int playerMoney;

void InitPlaying();

void ResetGame();

void UpdatePlaying();

void DrawPlaying();