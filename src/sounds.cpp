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
    Sound torcher;
    Sound thunder;
    Sound money;

    void InitSounds() {
        arrow_fly = LoadSound("assets/sound_effects/arrow_fly.wav");
        SetSoundVolume(arrow_fly, 0.5f);
        explosion = LoadSound("assets/sound_effects/explosion.wav");
        upgrade = LoadSound("assets/sound_effects/upgrade.wav");
        sell = LoadSound("assets/sound_effects/sell.wav");
        error = LoadSound("assets/sound_effects/error.wav");
        SetSoundVolume(error, 0.2f);
        place = LoadSound("assets/sound_effects/place.wav");
        fireball = LoadSound("assets/sound_effects/fireball.wav");
        SetSoundVolume(fireball, 0.1f);
        spider_spawn = LoadSound("assets/sound_effects/spider_spawn.wav");
        torcher = LoadSound("assets/sound_effects/torcher.wav");
        SetSoundVolume(torcher, 0.3f);
        thunder = LoadSound("assets/sound_effects/thunder.wav");
        SetSoundVolume(thunder, 0.02f);
        money = LoadSound("assets/sound_effects/money.wav");
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
        UnloadSound(torcher);
        UnloadSound(thunder);
        UnloadSound(money);
    }
}
