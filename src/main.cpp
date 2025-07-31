#include <iostream>
#include "raylib.h"
#include "play.h"
#include "options.h"
#include "tower.h"
#include "enemy.h"
#include "sounds.h"
#include "images.h"

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    const int screenWidthMid = screenWidth / 2;

    InitWindow(screenWidth, screenHeight, "Rampart Defenders");
    InitPlaying();

    InitAudioDevice();

    SoundManager::InitSounds();
    ImageHandler::InitImages();

    SetExitKey(0); // Prevents ESC key from closing the window

    SetTargetFPS(60);

    enum gameState {MENU, PLAYING, OPTIONS, EXIT};

    gameState state = MENU;

    while (!WindowShouldClose() && state != EXIT) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexture(ImageHandler::menuBackground, 0, 0, WHITE);

        switch (state) {
            case MENU: {
                const int screenWidthMid = GetScreenWidth() / 2;
                Vector2 mousePos = GetMousePosition();

                const float buttonWidth = 160.0f;
                const float buttonHeight = 40.0f;
                const float hoverScale = 1.1f;

                const int normalFontSize = 24;
                const int hoverFontSize = 28;

                struct Button {
                    Rectangle rect;
                    const char* text;
                };

                Button buttons[] = {
                    { { screenWidthMid - buttonWidth / 2, 325, buttonWidth, buttonHeight }, "Play" },
                    { { screenWidthMid - buttonWidth / 2, 425, buttonWidth, buttonHeight }, "Options" },
                    { { screenWidthMid - buttonWidth / 2, 525, buttonWidth, buttonHeight }, "Exit" }
                };

                for (int i = 0; i < 3; i++) {
                    Rectangle btn = buttons[i].rect;
                    const char* label = buttons[i].text;

                    bool hovered = CheckCollisionPointRec(mousePos, btn);
                    Rectangle drawRect = btn;
                    int fontSize = normalFontSize;

                    if (hovered) {
                        drawRect.width *= hoverScale;
                        drawRect.height *= hoverScale;
                        drawRect.x -= (drawRect.width - btn.width) / 2;
                        drawRect.y -= (drawRect.height - btn.height) / 2;
                        fontSize = hoverFontSize;

                        DrawRectangleRec(drawRect, DARKGRAY);
                    } else {
                        DrawRectangleRec(drawRect, DARKGRAY);
                    }

                    int textWidth = MeasureText(label, fontSize);
                    DrawText(
                        label,
                        drawRect.x + drawRect.width / 2 - textWidth / 2,
                        drawRect.y + drawRect.height / 2 - fontSize / 2,
                        fontSize,
                        WHITE
                    );
                }

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(mousePos, buttons[0].rect)) {
                        state = PLAYING;
                        ResetGame();
                    }
                    else if (CheckCollisionPointRec(mousePos, buttons[1].rect)) {
                        state = OPTIONS;
                        BackPressed = false;
                    }
                    else if (CheckCollisionPointRec(mousePos, buttons[2].rect)) {
                        state = EXIT;
                    }
                }

                const char* title = "Rampart Defenders";
                int titleFontSize = 75;
                DrawText(title, screenWidthMid - MeasureText(title, titleFontSize) / 2, 125, titleFontSize, DARKGRAY);

                break;
            }
            case PLAYING: {

                if (Paused) {
                    DrawPlaying();
                    break;
                }

                UpdatePlaying();
                DrawPlaying();

                if (IsWindowMinimized()) {
                    Paused = true;
                } else if (HomePressed) {
                    state = MENU;
                    HomePressed = false;
                    for (auto& tower : towers) {
                        tower->unloadFrames(); // Unload the textures for the rest of the towers if home button is pressed
                    }
                    for (auto& enemy : enemies) {
                        enemy->unloadFrames(); // Unload the textures for the rest of the enemies if home button is pressed
                    }
                }
                break;
            }
            case OPTIONS:
                UpdateOptions();
                DrawOptions();

                if (BackPressed) {
                    state = MENU;
                }
                break;
            case EXIT:
                break;
        }

        EndDrawing();
    }

    SoundManager::UnloadSounds();
    ImageHandler::UnloadImages();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}