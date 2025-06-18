#pragma once
#include "raylib.h"
#include <vector>

namespace ImageHandler {
    extern Texture2D archerIcon;
    extern Texture2D mageIcon;
    extern Texture2D torcherIcon;
    extern Texture2D stormshaperIcon;
    extern Texture2D wardrummerIcon;
    extern Texture2D goldmineIcon;
    extern std::vector<Texture2D> towerIcons;

    void InitImages();
    void UnloadImages();
}
