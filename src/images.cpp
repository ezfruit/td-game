#include "images.h"

namespace ImageHandler {
    Texture2D archerIcon;
    Texture2D mageIcon;
    Texture2D torcherIcon;
    Texture2D stormcallerIcon;
    Texture2D wardrummerIcon;
    Texture2D goldmineIcon;
    Texture2D archerPreview;
    Texture2D magePreview;
    Texture2D torcherPreview;
    Texture2D stormcallerPreview;
    Texture2D wardrummerPreview;
    std::vector<Texture2D> towerIcons;
    std::vector<Texture2D> previewIcons;
    std::vector<Texture2D> track;
    Texture2D gameBackground;
    Texture2D topUIBackground;
    Texture2D bottomUIBackground;
    Texture2D health;
    Texture2D gold;

    std::vector<Texture2D> LoadAnimationFrames(const std::string& name, int frameCount) {
        std::vector<Texture2D> frames;
        for (int i = 0; i < frameCount; i++) {
            std::string path = "assets/" + name + "_" + "animations/" + name + "_" + std::to_string(i) + ".png";
            frames.push_back(LoadTexture(path.c_str()));
        }
        return frames;
    }

    std::vector<Texture2D> LoadFrames(const std::string& name, int frameCount) {
        std::vector<Texture2D> frames;
        for (int i = 0; i < frameCount; i++) {
            std::string path = "assets/" + name + "/" + name + "_" + std::to_string(i) + ".png";
            frames.push_back(LoadTexture(path.c_str()));
        }
        return frames;
    }

    void InitImages() {
        archerIcon = LoadTexture("assets/icons/archer_icon.png");
        mageIcon = LoadTexture("assets/icons/mage_icon.png");
        torcherIcon = LoadTexture("assets/icons/torcher_icon.png");
        stormcallerIcon = LoadTexture("assets/icons/stormcaller_icon.png");
        wardrummerIcon = LoadTexture("assets/icons/wardrummer_icon.png");
        goldmineIcon = LoadTexture("assets/icons/goldmine_icon.png");
        towerIcons = { archerIcon , mageIcon, torcherIcon, stormcallerIcon, wardrummerIcon, goldmineIcon};
        archerPreview = LoadTexture("assets/archer_animations/archer_1.png");
        magePreview = LoadTexture("assets/mage_animations/mage_1.png");
        torcherPreview = LoadTexture("assets/torcher_animations/torcher_5.png");
        stormcallerPreview = LoadTexture("assets/stormcaller_animations/stormcaller_6.png");
        wardrummerPreview = LoadTexture("assets/wardrummer_animations/wardrummer_0.png");
        previewIcons = {archerPreview, magePreview, torcherPreview, stormcallerPreview, wardrummerPreview};
        track = LoadFrames("track", 9);
        gameBackground = LoadTexture("assets/gameplay_background.png");
        topUIBackground = LoadTexture("assets/top_background.png");
        bottomUIBackground = LoadTexture("assets/bottom_background.png");
        health = LoadTexture("assets/icons/health_icon.png");
        gold = LoadTexture("assets/icons/gold_icon.png");
    }

    void UnloadImages() {
        for (auto& icon : towerIcons) {
            UnloadTexture(icon);
        }
        for (auto& preview : previewIcons) {
            UnloadTexture(preview);
        }
        for (auto& tile : track) {
            UnloadTexture(tile);
        }
        UnloadTexture(gameBackground);
        UnloadTexture(topUIBackground);
        UnloadTexture(bottomUIBackground);
        UnloadTexture(health);
        UnloadTexture(gold);
    }
}
