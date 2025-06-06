#pragma once
#include <iostream>
#include <vector>

struct EnemySpawn {
    std::string type;
    float delay; // Time delay from previous spawn
};

struct GameWave {
    std::vector<EnemySpawn> enemies;
};

extern std::vector<GameWave> waveDefinitions;