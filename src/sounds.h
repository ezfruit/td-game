#pragma once
#include "raylib.h"

namespace SoundManager {
    extern Sound arrow;
    extern Sound explosion;
    extern Sound upgrade;
    extern Sound sell;
    extern Sound error;
    extern Sound place;
    extern Sound fireball;
    extern Sound spider_spawn;
    extern Sound fire_ignite;
    extern Sound thunder_flash;
    extern Sound money;
    extern Sound arcane_warden_spawn;

    void InitSounds();
    void UnloadSounds();
}
