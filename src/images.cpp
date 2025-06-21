#include "images.h"

namespace ImageHandler {
    Texture2D archerIcon;
    Texture2D mageIcon;
    Texture2D torcherIcon;
    Texture2D stormshaperIcon;
    Texture2D wardrummerIcon;
    Texture2D goldmineIcon;
    std::vector<Texture2D> towerIcons;
    void InitImages() {
        archerIcon = LoadTexture("assets/icons/archer_icon.png");
        mageIcon = LoadTexture("assets/icons/mage_icon.png");
        torcherIcon = LoadTexture("assets/icons/torcher_icon.png");
        stormshaperIcon = LoadTexture("assets/icons/stormshaper_icon.png");
        wardrummerIcon = LoadTexture("assets/icons/wardrummer_icon.png");
        goldmineIcon = LoadTexture("assets/icons/goldmine_icon.png");
        towerIcons = { archerIcon , mageIcon, torcherIcon, stormshaperIcon, wardrummerIcon, goldmineIcon};
        
    }

    std::vector<Texture2D> LoadAnimationFrames(const std::string& name, int frameCount) {
        std::vector<Texture2D> frames;
        for (int i = 0; i < frameCount; i++) {
            std::string path = "assets/" + name + "_" + "animations/" + name + "_" + std::to_string(i) + ".png";
            frames.push_back(LoadTexture(path.c_str()));
        }
        return frames;
    }

    void UnloadImages() {
        UnloadTexture(archerIcon);
        UnloadTexture(mageIcon);
        UnloadTexture(torcherIcon);
        UnloadTexture(stormshaperIcon);
        UnloadTexture(wardrummerIcon);
        UnloadTexture(goldmineIcon);
    }
}
