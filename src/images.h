#pragma once
#include "raylib.h"
#include <vector>
#include <string>
#include <unordered_map>
#include "play.h"

namespace ImageHandler {
    extern Texture2D archerIcon;
    extern Texture2D mageIcon;
    extern Texture2D torcherIcon;
    extern Texture2D stormcallerIcon;
    extern Texture2D wardrummerIcon;
    extern Texture2D goldmineIcon;
    extern std::vector<Texture2D> towerIcons;
    extern Texture2D archerPreview;
    extern Texture2D magePreview;
    extern Texture2D torcherPreview;
    extern Texture2D stormcallerPreview;
    extern Texture2D wardrummerPreview;
    extern Texture2D goldminePreview;
    extern std::vector<Texture2D> previewIcons;
    extern std::unordered_map<int, std::vector<Texture2D>> upgrades;
    extern std::vector<Texture2D> track;
    extern Texture2D gameBackground;
    extern Texture2D topUIBackground;
    extern Texture2D bottomUIBackground;
    extern Texture2D health;
    extern Texture2D gold;

    std::vector<Texture2D> LoadAnimationFrames(const std::string& enemyType, int frameCount);
    std::vector<Texture2D> LoadFrames(const std::string& name, int frameCount);

    void InitImages();
    void UnloadImages();
}
