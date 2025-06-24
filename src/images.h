#pragma once
#include "raylib.h"
#include <vector>
#include <string>

namespace ImageHandler {
    extern Texture2D archerIcon;
    extern Texture2D mageIcon;
    extern Texture2D torcherIcon;
    extern Texture2D stormcallerIcon;
    extern Texture2D wardrummerIcon;
    extern Texture2D goldmineIcon;
    extern std::vector<Texture2D> towerIcons;

    void InitImages();
    std::vector<Texture2D> LoadAnimationFrames(const std::string& enemyType, int frameCount);
    void UnloadImages();
}
