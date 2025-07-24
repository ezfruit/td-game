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
    Texture2D goldminePreview;
    std::vector<Texture2D> towerIcons;
    std::vector<Texture2D> previewIcons;
    std::unordered_map<int, std::vector<Texture2D>> upgrades;
    std::vector<Texture2D> track;
    Texture2D gameBackground;
    Texture2D topUIBackground;
    Texture2D bottomUIBackground;
    Texture2D menuBackground;
    Texture2D health;
    Texture2D gold;

    void LoadUpgradeImages(const std::vector<std::string>& towers) {
        for (int i = 0; i < towers.size(); i++) {
            for (int j = 1; j <= 5; j++) {
                std::string path = "assets/upgrades/" + towers[i] + "/level_" + std::to_string(j) + ".png";
                Texture2D image = LoadTexture(path.c_str());
                upgrades[i+1].push_back(image);
            }
        }
    }

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
        goldminePreview = LoadTexture("assets/goldmine_animations/goldmine_0.png");
        previewIcons = {archerPreview, magePreview, torcherPreview, stormcallerPreview, wardrummerPreview, goldminePreview};
        upgrades = {{1, {}}, {2, {}}, {3, {}}, {4, {}}, {5, {}}, {6, {}}};
        LoadUpgradeImages(towerNames);
        track = LoadFrames("track", 9);
        gameBackground = LoadTexture("assets/gameplay_background.png");
        topUIBackground = LoadTexture("assets/top_background.png");
        bottomUIBackground = LoadTexture("assets/bottom_background.png");
        menuBackground = LoadTexture("assets/menu_background.png");
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
        for (auto& [towerId, textures] : upgrades) {
            for (Texture2D& tex : textures) {
                UnloadTexture(tex);
            }
        }
        upgrades.clear();
        for (auto& tile : track) {
            UnloadTexture(tile);
        }
        UnloadTexture(gameBackground);
        UnloadTexture(topUIBackground);
        UnloadTexture(bottomUIBackground);
        UnloadTexture(menuBackground);
        UnloadTexture(health);
        UnloadTexture(gold);
    }
}
