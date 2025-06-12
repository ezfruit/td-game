#include "wave.h"

std::vector<GameWave> waveDefinitions = {
    // Wave 1
    {
        {
            { "Slime", 0.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f }
        }
    },
    // Wave 2
    {
        {
            { "Slime", 0.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f }
        }
    },
    // Wave 3
    {
        {
            { "Slime", 0.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Slime", 0.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
        }
    },

    // Wave 4
    {
        {
            { "Slime", 0.0f },
            { "Slime", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
        }
    },

    // Wave 5
    {
        {
            { "Knight", 0.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Brute", 1.0f},
            { "Brute", 3.0f},
        }
    },

    // Wave 6
    {
        {
            { "Fire Imp", 0.0f },
            { "Fire Imp", 1.0f},
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f},
            { "Fire Imp", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
        }
    },

    // Wave 7
    {
        {
            { "Brute", 0.0f},
            { "Brute", 3.0f},
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.0f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.0f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.0f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Slime", 0.5f },
            { "Slime", 0.5f },
            { "Slime", 0.5f },
            { "Slime", 0.5f },
            { "Slime", 0.5f },
            { "Fire Imp", 0.5f },
            { "Fire Imp", 0.5f },
        }
    },

    // Wave 8
    {
        {
            { "Slime", 0.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Brute", 1.0f },
            { "Brute", 3.0f },
            { "Spider Queen", 3.0f },
        }
    },

    // Wave 9
    {
        {
            { "Brute", 1.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
        }
    },

};

const size_t totalWaves = waveDefinitions.size();