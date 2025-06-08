#include "raylib.h"
#include "raymath.h"
#include "play.h"
#include "tower.h"
#include "wave.h"
#include "projectile.h"
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

static std::vector<Vector2> trackPoints;
static std::vector<std::unique_ptr<Tower>> towers;
static std::vector<std::unique_ptr<Enemy>> enemies;
static std::vector<Projectile> projectiles;

static int waveNumber = 0;

int playerMoney = 500;

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
bool Paused = false;

static float spawnTimer = 0.0f;
static int spawnIndex = 0;

static bool spawning = false;

static Tower* selectedTower = nullptr;

std::unordered_map<int, int> costs = {
    {1, 200},
    {2, 400},
    {3, 20},
    {4, 20},
    {5, 20},
    {6, 20}
};

std::unordered_map<std::string, std::vector<int>> upgradeCosts = {
    {"Archer", {400, 1000, 2500, 4000}},
    {"Mage", {300, 800, 3000, 6000}}
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

    Paused = false;

    spawning = false;
    spawnIndex = 0;
    spawnTimer = 0.0f;

    selectedTower = nullptr;

    towers.clear();
    enemies.clear();
}

// This function checks if the mouse is currently on the track (returns true if so)
bool IsOnTrack(Vector2 pos, const std::vector<Vector2>& trackPoints) {
    const float buffer = 40.0f;  // minimum distance from track line

    for (size_t i = 0; i < trackPoints.size() - 1; ++i) {
        Vector2 a = trackPoints[i];
        Vector2 b = trackPoints[i + 1];

        Vector2 ab = Vector2Subtract(b, a);
        Vector2 ap = Vector2Subtract(pos, a);
        float t = Vector2DotProduct(ap, ab) / Vector2LengthSqr(ab);
        t = fmax(0.0f, fmin(1.0f, t)); 
        Vector2 closest = Vector2Add(a, Vector2Scale(ab, t));

        if (Vector2Distance(pos, closest) < buffer)
            return true;
    }
    return false;
}

// This functions checks if the mouse is currently on a placed tower (returns true if so)
bool IsOnTower(Vector2 pos, const std::vector<std::unique_ptr<Tower>>& towers) {
    const float towerSize = 40.0f; // size of each tower's square

    for (const auto& tower : towers) {
        if (Vector2Distance(pos, tower->getPosition()) < towerSize)
            return true;
    }
    return false;
}

// This function checks if the mouse is currently within the placeable field bounds (returns true if so)
bool IsWithinBounds(Vector2 pos) {
    
    if (pos.y > 40 && pos.y < 560) {
        return true;
    }
    return false;
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

    for (auto& tower : towers) {
        tower->attack(deltaTime, enemies, projectiles);
    }

    for (int i = enemies.size() - 1; i >= 0; --i) {
        Vector2 pos = enemies[i]->getPosition();
        float distanceToGoal = Vector2Distance(pos, goal);

        if (distanceToGoal < 5.0f) {
            playerHealth -= enemies[i]->getHealth();

            if (playerHealth <= 0) {
                GameOver = true;
            }

            enemies.erase(enemies.begin() + i);
            continue;
        }

        if (enemies[i]->isAlive()) {
            allDefeated = false;
        } else {
            enemies.erase(enemies.begin() + i);
            continue;
        }

        enemies[i]->update(deltaTime, trackPoints); // Move enemy along path
    }

    if (waveInProgress && allDefeated && !spawning) {
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
                } else if (type == "Knight") {
                    enemy = std::make_unique<Knight>();
                }

                if (enemy) {
                    enemy->setPosition(trackPoints[0]);
                    enemies.push_back(std::move(enemy));
                }

                spawnTimer = 0.0f;
                spawnIndex++;
            }
        } else {
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
            selectedTower = false;
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && GetMousePosition().y > 40 && GetMousePosition().y < 560 ) {
        Vector2 mouse = GetMousePosition();
        selectedTower = nullptr;
        for (auto& tower : towers) {
            if (CheckCollisionPointRec(mouse, { tower->getPosition().x - 20, tower->getPosition().y - 20, 40, 40 })) {
                selectedTower = tower.get();
                break;
            }
        }
    }

    if (selectedTowerIndex != -1 && IsKeyPressed(KEY_X)) {
        selectedTowerIndex = -1;
        isPlacingTower = false;
        ShowCursor();
    }

    if (selectedTowerIndex != -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();

        if (mousePos.y > 40 && mousePos.y < 560 && !IsOnTrack(mousePos, trackPoints) && !IsOnTower(mousePos, towers)) {
            
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

    // When a tower is currently being placed
    if (isPlacingTower) {
        int range = 0;
        previewPosition = GetMousePosition();

        HideCursor();

        switch (selectedTowerIndex) {
            case 1: range = 150; break; // Archer
            case 2: range = 100; break; // Mage
        }

        Vector2 mousePos = GetMousePosition();

        bool invalidPlacement = IsOnTrack(mousePos, trackPoints) || IsOnTower(mousePos, towers) || !IsWithinBounds(mousePos);

        Color color;
        invalidPlacement ? color = Fade(RED, 0.3f) : color = Fade(BLUE, 0.3f);
        
        DrawCircle(previewPosition.x, previewPosition.y, range, color);
        DrawRectangleV({ previewPosition.x - 20, previewPosition.y - 20 }, { 40, 40 }, DARKGRAY);
        
    }

    for (const auto& tower : towers) {
        Vector2 pos = tower->getPosition();
        DrawRectangleV({ pos.x - 20, pos.y - 20 }, { 40, 40 }, DARKGRAY);
    }

    if (isPlacingTower) {
        DrawText("Press X to cancel", GetScreenWidth() / 2 - 100, GetScreenHeight() - 190, 20, RED);
    }

    if (selectedTower) {
        DrawCircleV(selectedTower->getPosition(), selectedTower->getRange(), Fade(BLUE, 0.3f));
    }

    DrawRectangle(0, 560, GetScreenWidth(), 160, LIGHTGRAY); // Bottom Gray Rectangle UI

    // When a tower is selected (clicked on in the field)
    if (selectedTower) {
        int infoX = GetScreenWidth() / 2 + 25;
        int infoY = GetScreenHeight() - 160;
        DrawRectangle(infoX, infoY, 200, 160, LIGHTGRAY);
        DrawText(selectedTower->getName().c_str(), infoX + 10, infoY + 10, 24, DARKGRAY);
        DrawText(TextFormat("Level: %d", selectedTower->getLevel()), infoX + 10, infoY + 45, 20, DARKGRAY);
        DrawText(TextFormat("Damage Dealt: %d", selectedTower->getTotalDamageDealt()), infoX + 10, infoY + 75, 20, DARKGRAY);

        Rectangle upgradeBtn = { (float)(infoX + 10), (float)(infoY + 110), 110, 30 };
        Rectangle sellBtn = { (float)(infoX + 130), (float)(infoY + 110), 110, 30 };

        DrawRectangleRec(upgradeBtn, GREEN);
        DrawRectangleRec(sellBtn, RED);

        DrawText("Upgrade (E)", upgradeBtn.x + 10, upgradeBtn.y + 8, 16, WHITE);
        DrawText("Sell (X)", sellBtn.x + 29, sellBtn.y + 8, 16, WHITE);

        DrawText(TextFormat("Sell Value: %d", selectedTower->getValue()), GetScreenWidth() - 175, GetScreenHeight() - 30, 20, WHITE);

        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_X) || IsKeyPressed(KEY_E)) {
            int upgradeCost = upgradeCosts[selectedTower->getName()][selectedTower->getLevel()-1];
            if ((IsKeyPressed(KEY_E) || CheckCollisionPointRec(mouse, upgradeBtn)) && upgradeCost <= playerMoney) {
                selectedTower->upgrade(upgradeCost);
                playerMoney -= upgradeCost;
            }
            else if (CheckCollisionPointRec(mouse, sellBtn) || IsKeyPressed(KEY_X)) {
                playerMoney += selectedTower->getValue();
                towers.erase(std::remove_if(towers.begin(), towers.end(),
                    [&](const std::unique_ptr<Tower>& t) { return t.get() == selectedTower; }),
                    towers.end());
                selectedTower = nullptr;
            }
        }
    }

    DrawRectangle(0, 0, GetScreenWidth(), 40, LIGHTGRAY); // Top Gray Rectangle UI

    DrawText(TextFormat("Health: %d", playerHealth), 20, 10, 20, RED);
    DrawText(TextFormat("$ %d", playerMoney), 200, 10, 20, GREEN);
    DrawText(TextFormat("Wave: %d", waveNumber), 600, 10, 20, DARKGRAY);

    for (size_t i = 0; i < trackPoints.size() - 1; i++) {
        DrawLineV(trackPoints[i], trackPoints[i + 1], DARKGRAY);
    }

    for (const auto& point : trackPoints) {
        DrawCircleV(point, 6, LIGHTGRAY);
    }

    Vector2 mousePos = GetMousePosition();

    for (const auto& enemy : enemies) {

        Vector2 enemyPos = enemy->getPosition();

        float hoverDistance = 15.0f;

        std::string name = enemy->getName();

        if (name == "Slime") {
            DrawCircleV(enemyPos, 10, GREEN);
        } else if (name == "Knight") {
            DrawCircleV(enemyPos, 10, GRAY);
        }

        if (Vector2Distance(mousePos, enemyPos) <= hoverDistance) {
            // Draw enemy health bar
            float healthBarWidth = 40.0f;
            float healthBarHeight = 6.0f;
            float healthPercent = static_cast<float>(enemy->getHealth()) / enemy->getMaxHealth();

            Vector2 barPos = { enemyPos.x - healthBarWidth / 2, enemyPos.y - 25 };
            DrawRectangle(barPos.x, barPos.y, healthBarWidth, healthBarHeight, LIGHTGRAY); // background
            DrawRectangle(barPos.x, barPos.y, healthBarWidth * healthPercent, healthBarHeight, LIME); // health

            // Draw enemy name and health as text
            std::string info = enemy->getName() + ": " + std::to_string(enemy->getHealth());
            DrawText(info.c_str(), barPos.x, barPos.y - 18, 14, DARKGRAY);

        }
    }

    int numSlots = 6;
    int slotSize = 80; 
    int spacing = 20;
    int startX = 20;
    int y = 600;

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
                selectedTower = false;
                HideCursor();
            }
        }
    }

    Rectangle pauseButton = {GetScreenWidth() - 100.0f, 0.0f, 40.0f, 40.0f};

    // Detect button click
    if ((IsKeyPressed(KEY_ESCAPE)) || (CheckCollisionPointRec(GetMousePosition(), pauseButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
        Paused = !Paused;
    }

    DrawRectangleLinesEx(pauseButton, 2, DARKGRAY);

    // Draw two vertical bars inside the button (the pause icon)
    float barWidth = 5.0f;
    float gap = 10.0f;

    DrawRectangle(pauseButton.x + gap, pauseButton.y + 6, barWidth, pauseButton.height - 12, DARKGRAY);
    DrawRectangle(pauseButton.x + gap + barWidth + gap, pauseButton.y + 6, barWidth, pauseButton.height - 12, DARKGRAY);

    DrawLineV({640, 560}, {640, 720}, DARKGRAY);
}
