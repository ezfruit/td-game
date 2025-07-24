#include "options.h"

float volume = 0.5f; // Initial volume [0.0f - 1.0f]

bool BackPressed = false;
bool InfiniteGold = false;
bool AllowPausing = false;

void UpdateOptions() {
}

void DrawOptions() {
    ClearBackground(RAYWHITE);
    DrawTexture(ImageHandler::menuBackground, 0, 0, WHITE);

    const float sliderWidth = 300;
    const float sliderHeight = 20;
    const float sliderX = GetScreenWidth() / 2 - sliderWidth / 2;
    const float sliderY = 250;

    const int screenWidthMid = GetScreenWidth() / 2;
    Vector2 mousePos = GetMousePosition();

    const char* settingsLabel = "Settings";
    DrawText(settingsLabel, screenWidthMid - MeasureText(settingsLabel, 60) / 2, 100, 60, DARKGRAY);

    bool hovering = CheckCollisionPointRec(mousePos, { sliderX, sliderY, sliderWidth, sliderHeight });

    if (hovering && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        float relativeX = mousePos.x - sliderX;
        volume = Clamp(relativeX / sliderWidth, 0.0f, 1.0f);
        SetMasterVolume(volume);
    }

    DrawText(
        "Master Volume",
        sliderX + sliderWidth / 2 - MeasureText("Master Volume", 20) / 2,
        sliderY - 40,
        20,
        DARKGRAY
    );

    DrawRectangle(sliderX, sliderY, sliderWidth, sliderHeight, WHITE); // The background bar
    DrawRectangle(sliderX, sliderY, sliderWidth * volume, sliderHeight, DARKBLUE); // The filled bar
    DrawCircle(sliderX + sliderWidth * volume, sliderY + sliderHeight / 2, 10, BLUE);

    std::string percentText = std::to_string((int)(volume * 100 + 0.5f)) + "%";
    int percentTextWidth = MeasureText(percentText.c_str(), 20);
    DrawText(
        percentText.c_str(),
        sliderX + sliderWidth / 2 - percentTextWidth / 2,
        sliderY + sliderHeight + 10,
        20,
        BLACK
    );

    const char* cheatsLabel = "Cheats";
    DrawText(cheatsLabel, screenWidthMid - MeasureText(cheatsLabel, 60) / 2, 350, 60, DARKGRAY);
 
    const float buttonWidth = 120;
    const float buttonHeight = 40;
    const float hoverScale = 1.1f;

    int goldFontSize = 24;

    Rectangle infiniteGoldBtn = { screenWidthMid - buttonWidth, 450, buttonWidth * 2, buttonHeight };

    bool infGoldhovered = CheckCollisionPointRec(mousePos, infiniteGoldBtn);

    Rectangle bigGoldRect = infiniteGoldBtn;

    Color goldRect = InfiniteGold ? GREEN: RED;

    if (infGoldhovered) {
        bigGoldRect.width *= hoverScale;
        bigGoldRect.height *= hoverScale;
        bigGoldRect.x -= (bigGoldRect.width - infiniteGoldBtn.width) / 2;
        bigGoldRect.y -= (bigGoldRect.height - infiniteGoldBtn.height) / 2;
        goldFontSize = 28;
        DrawRectangleRec(bigGoldRect, goldRect);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            InfiniteGold = !InfiniteGold;
        }
    } else {
        DrawRectangleRec(bigGoldRect, goldRect);
    }

    const char* goldLabel = InfiniteGold ? "Max Gold: On": "Max Gold: Off";

    int textWidth = MeasureText(goldLabel, goldFontSize);
    DrawText(
        goldLabel,
        bigGoldRect.x + bigGoldRect.width / 2 - textWidth / 2,
        bigGoldRect.y + bigGoldRect.height / 2 - goldFontSize / 2,
        goldFontSize,
        WHITE
    );

    int pauseFontSize = 24;

    Rectangle pausingBtn = { screenWidthMid - buttonWidth, 500, buttonWidth * 2, buttonHeight };

    bool pauseHovered = CheckCollisionPointRec(mousePos, pausingBtn);

    Rectangle bigPauseRect = pausingBtn;

    Color pauseRect = AllowPausing ? GREEN: RED;

    if (pauseHovered) {
        bigPauseRect.width *= hoverScale;
        bigPauseRect.height *= hoverScale;
        bigPauseRect.x -= (bigPauseRect.width - pausingBtn.width) / 2;
        bigPauseRect.y -= (bigPauseRect.height - pausingBtn.height) / 2;
        pauseFontSize = 28;
        DrawRectangleRec(bigPauseRect, pauseRect);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            AllowPausing = !AllowPausing;
        }
    } else {
        DrawRectangleRec(bigPauseRect, pauseRect);
    }

    const char* pauseLabel = AllowPausing ? "Allow Pausing: On": "Allow Pausing: Off";
    textWidth = MeasureText(pauseLabel, pauseFontSize);
    DrawText(
        pauseLabel,
        bigPauseRect.x + bigPauseRect.width / 2 - textWidth / 2,
        bigPauseRect.y + bigPauseRect.height / 2 - pauseFontSize / 2,
        pauseFontSize,
        WHITE
    );

    int backTextFontSize = 24;

    Rectangle backBtn = { screenWidthMid - buttonWidth / 2, 600, buttonWidth, buttonHeight };

    bool hovered = CheckCollisionPointRec(mousePos, backBtn);
    Rectangle drawRect = backBtn;

    if (hovered) {
        drawRect.width *= hoverScale;
        drawRect.height *= hoverScale;
        drawRect.x -= (drawRect.width - backBtn.width) / 2;
        drawRect.y -= (drawRect.height - backBtn.height) / 2;
        backTextFontSize = 28;
        DrawRectangleRec(drawRect, DARKGRAY);
    } else {
        DrawRectangleRec(drawRect, DARKGRAY);
    }

    const char* backLabel = "Back";
    textWidth = MeasureText(backLabel, backTextFontSize);
    DrawText(
        backLabel,
        drawRect.x + drawRect.width / 2 - textWidth / 2,
        drawRect.y + drawRect.height / 2 - backTextFontSize / 2,
        backTextFontSize,
        WHITE
    );
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hovered) {
        BackPressed = true;
    }
}
