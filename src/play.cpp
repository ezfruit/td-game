#include "raylib.h"
#include "play.h"
#include <vector>
#include <iostream>
#include "tower.h"
#include <unordered_map>

static std::vector<Vector2> trackPoints;
static std::vector<std::unique_ptr<Tower>> towers;
static std::vector<std::unique_ptr<Enemy>> enemies;
static int waveNumber = 0;
static int playerMoney = 500;
static int playerHealth = 100;
static int income = 500;
static bool waveInProgress = false;
static float waveDuration = 30.0f;
static float waveCooldown = 10.0f;
static float waveCooldownTimer = 0.0f;
static int selectedTowerIndex = -1;

static bool isPlacingTower = false;
static Vector2 previewPosition;

std::unordered_map<int, int> costs = {
    {1, 200},
    {2, 400},
    {3, 20},
    {4, 20},
    {5, 20},
    {6, 20}
};

// Creates the track
void InitPlaying() {
    trackPoints.push_back({ 0, 100 });
    trackPoints.push_back({ 300, 100 });
    trackPoints.push_back({ 300, 300 });
    trackPoints.push_back({ 100, 300 });
    trackPoints.push_back({ 100, 500 });
    trackPoints.push_back({ 500, 500 });
    trackPoints.push_back({ 500, 400 });
    trackPoints.push_back({ 600, 400 });
    trackPoints.push_back({ 600, 100 });
    trackPoints.push_back({ 850, 100 });
    trackPoints.push_back({ 850, 500 });
    trackPoints.push_back({ 1100, 500 });
    trackPoints.push_back({ 1100, 300 });
    trackPoints.push_back({ 1280, 300 });
}

// Logic to start the next wave
void StartNextWave() {
    waveNumber++;
    waveInProgress = true;
    waveCooldownTimer = 0.0f;
    enemies.clear();

    if (waveNumber > 1) {
        playerMoney += income;
        income += 50;
    }

    for (int i = 0; i < waveNumber * 3; ++i) {
        auto slime = std::make_unique<Slime>();
        slime->setPosition(trackPoints[0]); 
        enemies.push_back(std::move(slime));

        if (waveNumber > 2) {
            enemies.push_back(std::make_unique<Armored_Knight>());
        }
    }

    std::cout << "Wave " << waveNumber << " started!\n";
}

// Game logic updates
void UpdatePlaying() {

    bool allDefeated = true;
    float deltaTime = GetFrameTime();
    for (const auto& enemy : enemies) {
        enemy->update(deltaTime, trackPoints);
        if (enemy->isAlive()) {
            allDefeated = false;
        }
    }

    if (waveInProgress && allDefeated) {
        waveInProgress = false;
        waveCooldownTimer = 0.0f;
    }

    waveCooldownTimer += GetFrameTime();

    if (!waveInProgress) {
        if (waveCooldownTimer >= waveCooldown) {
            StartNextWave();
        }
    } else {
        if (waveCooldownTimer >= waveDuration) {
            StartNextWave();
        }
    }

    for (int i = 1; i <= 6; i++) {
        if (IsKeyPressed(KEY_ONE + (i - 1)) && costs[i] <= playerMoney) {
            selectedTowerIndex = i;
            isPlacingTower = true;
        }
    }

    if (selectedTowerIndex != -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();

        if (mousePos.y > 40 && mousePos.y < 560) {
            
            switch (selectedTowerIndex) {
                case 1:
                    towers.push_back(std::make_unique<Archer>(mousePos));
                    playerMoney -= 200;
                    std::cout << "Placed Archer" << '\n';
                    break;
                case 2:
                    towers.push_back(std::make_unique<Mage>(mousePos));
                    playerMoney -= 400;
                    std::cout << "Placed Mage" << '\n';
                    break;
                
            }

            selectedTowerIndex = -1;
            isPlacingTower = false;
            ShowCursor();
        }
    }

}

// Draw the game
void DrawPlaying() {
    ClearBackground(RAYWHITE);

    DrawRectangle(0, 0, GetScreenWidth(), 40, LIGHTGRAY); 

    DrawText(TextFormat("Health: %d", playerHealth), 20, 10, 20, RED);
    DrawText(TextFormat("$ %d", playerMoney), 200, 10, 20, GREEN);
    DrawText(TextFormat("Wave: %d", waveNumber), 600, 10, 20, DARKGRAY);

    if (isPlacingTower) {
        int range = 0;
        previewPosition = GetMousePosition();

        HideCursor();

        switch (selectedTowerIndex) {
            case 1: range = 150; break; // Archer
            case 2: range = 100; break; // Mage
        }

        DrawCircle(previewPosition.x, previewPosition.y, range, BLUE);
        DrawRectangleV({ previewPosition.x - 20, previewPosition.y - 20 }, { 40, 40 }, DARKGRAY);
        
    }

    for (size_t i = 0; i < trackPoints.size() - 1; i++) {
        DrawLineV(trackPoints[i], trackPoints[i + 1], DARKGRAY);
    }

    for (const auto& point : trackPoints) {
        DrawCircleV(point, 6, LIGHTGRAY);
    }

    for (const auto& tower : towers) {
        Vector2 pos = tower->getPosition();
        DrawRectangleV({ pos.x - 20, pos.y - 20 }, { 40, 40 }, DARKGRAY);
    }

    for (const auto& enemy : enemies) {
        DrawCircleV(enemy->getPosition(), 10, GREEN);
    }

    DrawRectangle(0, 560, GetScreenWidth(), 160, LIGHTGRAY);

    int numSlots = 6;
    int slotSize = 80; 
    int spacing = 20;
    int startX = 20;
    int y = 600;

    Vector2 mousePos = GetMousePosition();

    for (int i = 0; i < numSlots; i++) {
        int x = startX + i * (slotSize + spacing);

        Rectangle slotRect = { (float)x, (float)y, (float)slotSize, (float)slotSize };
        DrawRectangleRec(slotRect, DARKGRAY);

        if (CheckCollisionPointRec(mousePos, slotRect)) {
            Color borderColor = (selectedTowerIndex == i + 1) ? RED : GOLD;
            DrawRectangleLinesEx(slotRect, 3, borderColor);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && costs[i+1] <= playerMoney) {
                selectedTowerIndex = i + 1;
                isPlacingTower = true;
                HideCursor();
            }
        }
    }

    DrawLineV({640, 560}, {640, 720}, DARKGRAY);
}
