#include <iostream>
#include "raylib.h"
#include "play.h"
#include "options.h"
#include "tower.h"
#include "enemy.h"
#include "sounds.h"

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    const int screenWidthMid = screenWidth / 2;

    InitWindow(screenWidth, screenHeight, "Rampart Defenders");
    InitPlaying();

    InitAudioDevice();

    SoundManager::InitSounds();

    SetExitKey(0); // Prevents ESC key from closing the window

    SetTargetFPS(60);

    enum gameState {MENU, PLAYING, GAMEOVER, OPTIONS, EXIT};

    gameState state = MENU;

    while (!WindowShouldClose() && state != EXIT) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (state) {
            case MENU: {

                Rectangle playBtn = { screenWidthMid - 65, 290, 120, 40 };
                Rectangle optionsBtn = { screenWidthMid - 85, 390, 160, 40 };
                Rectangle exitBtn = { screenWidthMid - 65, 490, 120, 40 };

                DrawRectangleRec(playBtn, LIGHTGRAY);
                DrawRectangleRec(optionsBtn, LIGHTGRAY);
                DrawRectangleRec(exitBtn, LIGHTGRAY);

                Vector2 mousePos = GetMousePosition();

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(mousePos, playBtn)) {
                        state = PLAYING;
                        ResetGame();
                    }
                    else if (CheckCollisionPointRec(mousePos, optionsBtn)) {
                        state = OPTIONS;
                    }
                    else if (CheckCollisionPointRec(mousePos, exitBtn)) {
                        state = EXIT;
                    }
                }

                DrawText("Rampart Defenders", screenWidthMid - 275, 150, 60, DARKGRAY);
                DrawText("Play", screenWidthMid - 30, 300, 24, DARKGRAY);
                DrawText("Options", screenWidthMid - 50, 400, 24, DARKGRAY);
                DrawText("Exit", screenWidthMid - 30, 500, 24, DARKGRAY);
                break;
            }
            case PLAYING: {

                if (Paused) {
                    DrawPlaying();
                    break;
                }

                UpdatePlaying();
                DrawPlaying();

                if (GameOver) {
                    state = GAMEOVER;
                } else if (IsWindowMinimized()) {
                    Paused = true;
                } else if (HomePressed) {
                    state = MENU;
                    HomePressed = false;
                }
                break;
            }
            case GAMEOVER: {

                Rectangle menuBtn = { screenWidthMid - 110, 340, 210, 40 };
                Rectangle exitBtn = { screenWidthMid - 70, 440, 120, 40 };

                DrawRectangleRec(menuBtn, LIGHTGRAY);
                DrawRectangleRec(exitBtn, LIGHTGRAY);

                Vector2 mousePos = GetMousePosition();

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(mousePos, menuBtn)) {
                        state = MENU;
                        GameOver = false;
                    }
                    else if (CheckCollisionPointRec(mousePos, exitBtn)) {
                        state = EXIT;
                    }
                }

                DrawText("GAME OVER!", screenWidthMid - 180, 150, 60, DARKGRAY);
                DrawText("The enemies slipped past your lines.", screenWidthMid - 340, 250, 40, DARKGRAY);
                DrawText("Return to Menu", screenWidthMid - 100, 350, 24, DARKGRAY);
                DrawText("Exit", screenWidthMid - 30, 450, 24, DARKGRAY);
                break;
            }
            case OPTIONS:
                UpdateOptions();
                DrawOptions();
                break;
            case EXIT:
                break;
        }

        EndDrawing();
    }

    SoundManager::UnloadSounds();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}