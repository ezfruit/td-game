#include "sounds.h"

namespace SoundManager {
    Sound arrow_fly;
    Sound explosion;
    Sound upgrade;
    Sound sell;
    Sound error;
    Sound place;
    Sound fireball;
    Sound spider_spawn;

    void InitSounds() {
        arrow_fly = LoadSound("assets/arrow_fly.wav");
        SetSoundVolume(arrow_fly, 0.5f);
        explosion = LoadSound("assets/explosion.wav");
        upgrade = LoadSound("assets/upgrade.wav");
        sell = LoadSound("assets/sell.wav");
        error = LoadSound("assets/error.wav");
        SetSoundVolume(error, 0.2f);
        place = LoadSound("assets/place.wav");
        fireball = LoadSound("assets/fireball.wav");
        SetSoundVolume(fireball, 0.1f);
        spider_spawn = LoadSound("assets/spider_spawn.wav");
    }

    void UnloadSounds() {
        UnloadSound(arrow_fly);
        UnloadSound(explosion);
        UnloadSound(upgrade);
        UnloadSound(sell);
        UnloadSound(error);
        UnloadSound(place);
        UnloadSound(fireball);
        UnloadSound(spider_spawn);
    }
}
