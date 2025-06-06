#include "raylib.h"
#include "raymath.h"
#include "play.h"
#include "tower.h"
#include "wave.h"
#include <vector>
#include <iostream>
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
static float waveCooldown = 3.0f;
static float waveCooldownTimer = 0.0f;
static int selectedTowerIndex = -1;

static bool isPlacingTower = false;
static Vector2 previewPosition;

bool GameOver = false;

static float spawnTimer = 0.0f;
static int spawnIndex = 0;

static bool spawning = false;

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

void ResetGame() {
    waveNumber = 0;
    playerMoney = 500;
    playerHealth = 100;
    income = 500;
    waveInProgress = false;
    waveDuration = 30.0f;
    waveCooldown = 3.0f;
    waveCooldownTimer = 0.0f;
    selectedTowerIndex = -1;

    spawning = false;
    spawnIndex = 0;
    spawnTimer = 0.0f;

    towers.clear();
    enemies.clear();
}

void SpawnEnemies() {

}

// Logic to start the next wave
void StartNextWave() {
    waveNumber++;
    waveInProgress = true;
    waveCooldownTimer = 0.0f;

    if (waveNumber > 1) {
        playerMoney += income;
        income += 50;
    }

    spawning = true;

    std::cout << "Wave " << waveNumber << " started!\n";
}

// Game logic updates
void UpdatePlaying() {

    bool allDefeated = true;
    float deltaTime = GetFrameTime();

    Vector2 goal = trackPoints.back();

    for (int i = enemies.size() - 1; i >= 0; --i) {
        Vector2 pos = enemies[i]->getPosition();
        float distanceToGoal = Vector2Distance(pos, goal);

        if (distanceToGoal < 5.0f) {
            std::cout << "Went in" << '\n';
            playerHealth -= enemies[i]->getHealth();

            if (playerHealth <= 0) {
                GameOver = true;
            }

            enemies.erase(enemies.begin() + i);
            continue;
        }

        if (enemies[i]->isAlive()) {
            allDefeated = false;
        }

        enemies[i]->update(deltaTime, trackPoints); // Move enemy along path
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

    if (spawning) {
        const GameWave& currentWave = waveDefinitions[waveNumber - 1];

        if (spawnIndex < currentWave.enemies.size()) {
            spawnTimer += GetFrameTime();

            if (spawnTimer >= currentWave.enemies[spawnIndex].delay) {

                std::string type = currentWave.enemies[spawnIndex].type;

                std::unique_ptr<Enemy> enemy;
                if (type == "Slime") {
                    enemy = std::make_unique<Slime>();
                } else if (type == "Armored_Knight") {
                    enemy = std::make_unique<Armored_Knight>();
                } // Add more types here

                if (enemy) {
                    enemy->setPosition(trackPoints[0]);
                    enemies.push_back(std::move(enemy));
                    std::cout << "Enemy detected" << '\n';
                    std::cout << currentWave.enemies.size() << '\n';
                }

                spawnTimer = 0.0f;
                spawnIndex++;
            }
        } else {
            std::cout << "Empty" << '\n';
            spawning = false;
            spawnIndex = 0;
            spawnTimer = 0.0f;
            waveCooldownTimer = 0.0f;
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

    DrawRectangle(0, 0, GetScreenWidth(), 40, LIGHTGRAY); 

    DrawText(TextFormat("Health: %d", playerHealth), 20, 10, 20, RED);
    DrawText(TextFormat("$ %d", playerMoney), 200, 10, 20, GREEN);
    DrawText(TextFormat("Wave: %d", waveNumber), 600, 10, 20, DARKGRAY);

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
        std::string name = enemy->getName();
        if (name == "Slime") {
            DrawCircleV(enemy->getPosition(), 10, GREEN);
        } else if (name == "Armored_Knight") {
            DrawCircleV(enemy->getPosition(), 10, GRAY);
        }
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
