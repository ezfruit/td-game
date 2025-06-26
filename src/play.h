#pragma once
#include "raymath.h"
#include <vector>
#include <memory>
#include "effects.h"

extern std::vector<Vector2> trackPoints;

class Tower;
class Enemy;
class Projectile;

extern std::vector<std::shared_ptr<Tower>> towers;
extern std::vector<std::shared_ptr<Enemy>> enemies;
extern std::vector<std::shared_ptr<Projectile>> projectiles;
extern std::vector<Explosion> explosions;
extern std::vector<LightningBolt> lightningBolts;
extern std::vector<FloatingText> floatingTexts;

constexpr float TOWER_SIZE = 40.0f;

extern bool GameOver;
extern bool Paused;
extern bool HomePressed;

extern int playerMoney;

enum TargetMode { FIRST, LAST, STRONG, WEAK };
extern const char* targetModeNames[];

void InitPlaying();

void ResetGame();

void UpdatePlaying();

void DrawPlaying();