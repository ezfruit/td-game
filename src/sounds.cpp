#include "sounds.h"

namespace SoundManager {
    Sound arrow;
    Sound explosion;
    Sound upgrade;
    Sound sell;
    Sound error;
    Sound place;
    Sound fireball;
    Sound spider_spawn;
    Sound fire_ignite;
    Sound thunder_flash;
    Sound money;
    Sound arcane_warden_spawn;

    void InitSounds() {
        arrow = LoadSound("assets/sound_effects/arrow.wav");
        SetSoundVolume(arrow, 0.2f);
        explosion = LoadSound("assets/sound_effects/explosion.wav");
        SetSoundVolume(explosion, 0.2f);
        upgrade = LoadSound("assets/sound_effects/upgrade.wav");
        sell = LoadSound("assets/sound_effects/sell.wav");
        error = LoadSound("assets/sound_effects/error.wav");
        SetSoundVolume(error, 0.2f);
        place = LoadSound("assets/sound_effects/place.wav");
        fireball = LoadSound("assets/sound_effects/fireball.wav");
        SetSoundVolume(fireball, 0.1f);
        spider_spawn = LoadSound("assets/sound_effects/spider_spawn.wav");
        fire_ignite = LoadSound("assets/sound_effects/fire_ignite.wav");
        SetSoundVolume(fire_ignite, 0.3f);
        thunder_flash = LoadSound("assets/sound_effects/thunder_flash.wav");
        SetSoundVolume(thunder_flash, 0.02f);
        money = LoadSound("assets/sound_effects/money.wav");
        arcane_warden_spawn = LoadSound("assets/sound_effects/arcane_warden.wav");
        SetSoundVolume(arcane_warden_spawn, 0.5f);
    }

    void UnloadSounds() {
        UnloadSound(arrow);
        UnloadSound(explosion);
        UnloadSound(upgrade);
        UnloadSound(sell);
        UnloadSound(error);
        UnloadSound(place);
        UnloadSound(fireball);
        UnloadSound(spider_spawn);
        UnloadSound(fire_ignite);
        UnloadSound(thunder_flash);
        UnloadSound(money);
        UnloadSound(arcane_warden_spawn);
    }
}
