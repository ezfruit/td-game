#pragma once
#include "raylib.h"

namespace SoundManager {
    extern Sound arrow_fly;
    extern Sound explosion;
    extern Sound upgrade;
    extern Sound sell;
    extern Sound error;
    extern Sound place;

    void InitSounds();
    void UnloadSounds();
}
