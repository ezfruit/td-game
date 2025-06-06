#include <iostream>
#include "raylib.h"
#include "play.h"
#include "options.h"
#include "tower.h"
#include "enemy.h"

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    const int screenWidthMid = screenWidth / 2;

    InitWindow(screenWidth, screenHeight, "Rampart Defenders");
    InitPlaying();

    SetTargetFPS(60);

    enum gameState {MENU, PLAYING, OPTIONS, EXIT};

    gameState state = MENU;

    while (!WindowShouldClose() && state != EXIT) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (state) {
            case MENU: {

                Rectangle playBtn = { screenWidthMid - 60, 290, 120, 40 };
                Rectangle optionsBtn = { screenWidthMid - 80, 390, 160, 40 };
                Rectangle exitBtn = { screenWidthMid - 60, 490, 120, 40 };

                DrawRectangleRec(playBtn, LIGHTGRAY);
                DrawRectangleRec(optionsBtn, LIGHTGRAY);
                DrawRectangleRec(exitBtn, LIGHTGRAY);

                Vector2 mousePos = GetMousePosition();

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    if (CheckCollisionPointRec(mousePos, playBtn))
                    {
                        state = PLAYING;
                    }
                    else if (CheckCollisionPointRec(mousePos, optionsBtn))
                    {
                        state = OPTIONS;
                    }
                    else if (CheckCollisionPointRec(mousePos, exitBtn))
                    {
                        state = EXIT;
                    }
                }

                DrawText("Rampart Defenders", screenWidthMid - 275, 150, 60, DARKGRAY);
                DrawText("Play", screenWidthMid - 30, 300, 24, DARKGRAY);
                DrawText("Options", screenWidthMid - 50, 400, 24, DARKGRAY);
                DrawText("Exit", screenWidthMid - 30, 500, 24, DARKGRAY);
                break;
            }
            case PLAYING:
                UpdatePlaying();
                DrawPlaying();
                break;
            case OPTIONS:
                break;
            case EXIT:
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}