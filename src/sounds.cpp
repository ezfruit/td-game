#include "sounds.h"

namespace SoundManager {
    Sound arrow_fly;
    Sound explosion;
    Sound upgrade;
    Sound sell;

    void InitSounds() {
        arrow_fly = LoadSound("assets/arrow_fly.wav");
        explosion = LoadSound("assets/explosion.wav");
        upgrade = LoadSound("assets/upgrade.wav");
        sell = LoadSound("assets/sell.wav");
    }

    void UnloadSounds() {
        UnloadSound(arrow_fly);
        UnloadSound(explosion);
        UnloadSound(upgrade);
        UnloadSound(sell);
    }
}
