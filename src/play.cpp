#include "raylib.h"
#include "play.h"
#include "tower.h"
#include "wave.h"
#include "projectile.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include "sounds.h"

std::vector<Vector2> trackPoints;

static std::vector<std::shared_ptr<Tower>> towers;
std::vector<std::shared_ptr<Enemy>> enemies;
std::vector<std::shared_ptr<Projectile>> projectiles;

static int waveNumber = 0;

int playerMoney;

static int playerHealth = 100;
static int income = 500;
static bool waveInProgress = false;
static float waveDuration = 30.0f;
static float waveCooldown = 3.0f;
static float waveCooldownTimer = 0.0f;
static int selectedTowerIndex = -1;

static bool isPlacingTower = false;
static bool showNotEnoughMoney = false;
static float moneyMsgTimer = 0.0f;
static float moneyMsgDuration = 2.0f;

static Vector2 previewPosition;

bool GameOver = false;
bool Paused = false;
bool HomePressed = false;

static float spawnTimer = 0.0f;
static int spawnIndex = 0;

static bool spawning = false;

static Tower* selectedTower = nullptr;

std::vector<std::string> towerNames = {
    "None",       
    "Archer",     
    "Mage",      
    "Torcher",
    "Stormshaper",
    "Placeholder",
    "Placeholder"
};

std::vector<std::string> towerTypes = {
    "None",
    "Physical",
    "Magic",
    "Fire",
    "Air",
    "Placeholder",
    "Placeholder"
};

std::vector<std::string> towerRanges = {
    "None",
    "High",
    "Low",
    "Very Low",
    "Very High",
    "Placeholder",
    "Placeholder"
};

std::vector<std::string> towerTargetings = {
    "None",
    "Pierce",
    "Area of Effect",
    "Single",
    "Area of Effect",
    "Placeholder",
    "Placeholder"
};

std::unordered_map<int, int> costs = {
    {1, 200},
    {2, 400},
    {3, 700},
    {4, 3000},
    {5, 20},
    {6, 20}
};

std::unordered_map<std::string, std::vector<int>> upgradeCosts = {
    {"Archer", {250, 600, 2500, 4000, 0}},
    {"Mage", {300, 800, 3000, 6000, 0}},
    {"Torcher", {300, 1200, 4000, 10000, 0}},
    {"Stormshaper", {600, 1800, 6000, 15000, 0}}
};

struct Explosion {
    Vector2 position;
    float radius;
    float duration = 0.4f; // in seconds
    float timeAlive = 0.0f;
    bool active = true;
};

static std::vector<Explosion> explosions;

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
    playerMoney = 30000;
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

    showNotEnoughMoney = false;

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
bool IsOnTower(Vector2 pos, const std::vector<std::shared_ptr<Tower>>& towers) {
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

void ApplyAOEDamage(Projectile& projectile, Vector2 center, float radius, int damage, std::string type) {
    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        float dist = Vector2Distance(enemy->getPosition(), center);
        if (dist <= radius) {
            int prevHealth = enemy->getHealth();
            enemy->takeDamage(damage, type);
            projectile.markHit(enemy.get());
            int curHealth = enemy->getHealth();
            int damageDealt = prevHealth - curHealth;
            if (auto shooter = projectile.getSourceTower().lock()) {
                shooter->setTotalDamageDealt(damageDealt);
            }
            playerMoney += damageDealt;
        }
    }
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

    for (auto& projectile : projectiles) {

        if (!projectile->isActive()) continue;

        projectile->update(deltaTime, projectile->getSourceTower().lock());

        for (auto& enemy : enemies) {
            if (!enemy->isAlive()) continue;

            if (projectile->hasHit(enemy.get())) continue;

            float distance = Vector2Distance(projectile->getPosition(), enemy->getPosition());
            if (distance <= enemy->getRadius()) { // collision radius

                if (projectile->getAOERadius() > 0.0f) {
                    PlaySound(SoundManager::explosion);
                    ApplyAOEDamage(*projectile, projectile->getPosition(), projectile->getAOERadius(), projectile->getDamage(), projectile->getDamageType());
                    explosions.push_back({projectile->getPosition(), projectile->getAOERadius()});
                    projectile->deactivate();  // deactivate projectile because it exploded
                } else {
                    int prevHealth = enemy->getHealth();
                    enemy->takeDamage(projectile->getDamage(), projectile->getDamageType());
                    projectile->markHit(enemy.get());
                    int curHealth = enemy->getHealth();
                    int damageDealt = prevHealth - curHealth;
                    if (auto shooter = projectile->getSourceTower().lock()) {
                        shooter->setTotalDamageDealt(damageDealt);
                    }
                    playerMoney += damageDealt;
                    break;
                }
            }
        }
    }

    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](const std::shared_ptr<Projectile>& p) { return !p->isActive(); }), projectiles.end());

    for (auto& exp : explosions) {
        exp.timeAlive += deltaTime;
        if (exp.timeAlive >= exp.duration) {
            exp.active = false;
        }
    }

    explosions.erase(std::remove_if(explosions.begin(), explosions.end(), [](const Explosion& e) { return !e.active; }), explosions.end());

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
    } else if (!spawning) {
        if (waveCooldownTimer >= waveDuration) {
            StartNextWave();
        }
    }

    if (spawning && waveNumber <= totalWaves) {

        const GameWave& currentWave = waveDefinitions[waveNumber - 1];

        if (spawnIndex < currentWave.enemies.size()) {
            spawnTimer += GetFrameTime();

            if (spawnTimer >= currentWave.enemies[spawnIndex].delay) {

                std::string type = currentWave.enemies[spawnIndex].type;

                std::shared_ptr<Enemy> enemy;
                if (type == "Slime") {
                    enemy = std::make_shared<Slime>();
                } else if (type == "Knight") {
                    enemy = std::make_shared<Knight>();
                } else if (type == "Fire Imp") {
                    enemy = std::make_shared<Fire_Imp>();
                } else if (type == "Spider Queen") {
                    PlaySound(SoundManager::spider_spawn);
                    enemy = std::make_shared<Spider_Queen>();
                } else if (type == "Brute") {
                    enemy = std::make_shared<Brute>();
                } else if (type == "Arcane Shell") {
                    enemy = std::make_shared<Arcane_Shell>();
                } else if (type == "Flux") {
                    enemy = std::make_shared<Flux>();
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
        if (IsKeyPressed(KEY_ONE + (i - 1))) {
            if (costs[i] <= playerMoney) {
                selectedTowerIndex = i;
                isPlacingTower = true;
                selectedTower = nullptr;
            } else {
                PlaySound(SoundManager::error);
                showNotEnoughMoney = true;
                moneyMsgTimer = 0.0f;
            }
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
            
            PlaySound(SoundManager::place);

            switch (selectedTowerIndex) {
                case 1:
                    towers.push_back(std::make_shared<Archer>(mousePos));
                    playerMoney -= costs[1];
                    std::cout << "Placed Archer" << '\n';
                    break;
                case 2:
                    towers.push_back(std::make_shared<Mage>(mousePos));
                    playerMoney -= costs[2];
                    std::cout << "Placed Mage" << '\n';
                    break;
                case 3:
                    towers.push_back(std::make_shared<Torcher>(mousePos));
                    playerMoney -= costs[3];
                    std::cout << "Placed Torcher" << '\n';
                    break;
                case 4:
                    towers.push_back(std::make_shared<Stormshaper>(mousePos));
                    playerMoney -= costs[4];
                    std::cout << "Placed Stormshaper" << '\n';
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
            case 3: range = 75; break; // Torcher
            case 4: range = 300; break; // Stormshaper
        }

        Vector2 mousePos = GetMousePosition();

        bool invalidPlacement = IsOnTrack(mousePos, trackPoints) || IsOnTower(mousePos, towers) || !IsWithinBounds(mousePos);

        Color color;
        invalidPlacement ? color = Fade(RED, 0.3f) : color = Fade(BLUE, 0.3f);
        
        DrawCircle(previewPosition.x, previewPosition.y, range, color);
        DrawRectangleV({ previewPosition.x - 20, previewPosition.y - 20 }, { 40, 40 }, DARKGRAY);
        
    }

    if (showNotEnoughMoney) {
        moneyMsgTimer += GetFrameTime();

        // Calculate alpha for fade-out effect (starts fading after 1.5s)
        float alpha = 1.0f;
        if (moneyMsgTimer > 1.5f) {
            alpha = 1.0f - ((moneyMsgTimer - 1.5f) / 0.5f);  // fade out over 0.5s
            if (alpha < 0.0f) alpha = 0.0f;
        }

        // Set color with alpha
        Color fadedRed = {255, 0, 0, (unsigned char)(255 * alpha)};
        DrawText("Not enough money!", GetScreenWidth() / 2 - 100, 50, 20, fadedRed);

        if (moneyMsgTimer > moneyMsgDuration) {
            showNotEnoughMoney = false;
        }
    }

    for (const auto& tower : towers) {
        Vector2 pos = tower->getPosition();
        DrawRectangleV({ pos.x - 20, pos.y - 20 }, { 40, 40 }, DARKGRAY);
    }

    if (isPlacingTower) {

        showNotEnoughMoney = false;

        std::string tower;

        switch (selectedTowerIndex) {
            case 1: tower = "Archer"; break; // Archer
            case 2: tower = "Mage"; break; // Mage
            case 3: tower = "Torcher"; break; // Torcher
            case 4: tower = "Stormshaper"; break; // Stormshaper
        }

        std::string placingText = "Placing " + tower;
        int fontSize = 24;

        // Measure the width in pixels of the text
        int textWidth = MeasureText(placingText.c_str(), fontSize);

        // Center horizontally
        int x = GetScreenWidth() / 2 - textWidth / 2;

        DrawText(placingText.c_str(), x, 50, fontSize, BLUE);

        DrawText("Press X to cancel", GetScreenWidth() / 2 - 100, GetScreenHeight() - 190, 20, RED);
    }

    if (selectedTower) {
        DrawCircleV(selectedTower->getPosition(), selectedTower->getRange(), Fade(BLUE, 0.3f));
    }

    for (size_t i = 0; i < trackPoints.size() - 1; i++) {
        DrawLineV(trackPoints[i], trackPoints[i + 1], DARKGRAY);
    }

    for (const auto& point : trackPoints) {
        DrawCircleV(point, 6, LIGHTGRAY);
    }

    for (auto& proj : projectiles) {
        proj->draw();
    }

    for (auto& exp : explosions) {
        float alpha = 1.0f - (exp.timeAlive / exp.duration); // fade out
        Color color = Fade(ORANGE, alpha);
        DrawCircleV(exp.position, exp.radius, color);
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

        int upgradeCost = upgradeCosts[selectedTower->getName()][selectedTower->getLevel() - 1];
        int fontSize = 20;

        std::string costStr = TextFormat("Upgrade Cost: $%d", upgradeCost);
        int textWidth = MeasureText(costStr.c_str(), fontSize);

        // Center horizontally
        int x = GetScreenWidth() - 125 - (textWidth / 2);
        int y = GetScreenHeight() - 60;

        DrawText(costStr.c_str(), x, y, fontSize, LIME);

        std::string sellValueStr = TextFormat("Sell Value: $%d", selectedTower->getValue());
        textWidth = MeasureText(sellValueStr.c_str(), fontSize);

        x = GetScreenWidth() - 125 - (textWidth / 2);
        y = GetScreenHeight() - 30;

        DrawText(sellValueStr.c_str(), x, y, fontSize, WHITE);

        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_X) || IsKeyPressed(KEY_E)) {

            if (IsKeyPressed(KEY_E) || CheckCollisionPointRec(mouse, upgradeBtn)) {
                if (upgradeCost <= playerMoney) {
                    PlaySound(SoundManager::upgrade);
                    selectedTower->upgrade(upgradeCost);
                    playerMoney -= upgradeCost;
                } else {
                    PlaySound(SoundManager::error);
                    showNotEnoughMoney = true;
                    moneyMsgTimer = 0.0f;
                }
            }
            else if (CheckCollisionPointRec(mouse, sellBtn) || IsKeyPressed(KEY_X)) {
                PlaySound(SoundManager::sell);
                playerMoney += selectedTower->getValue();
                towers.erase(std::remove_if(towers.begin(), towers.end(),
                    [&](const std::shared_ptr<Tower>& t) { return t.get() == selectedTower; }),
                    towers.end());
                selectedTower = nullptr;
            }
        }
    }

    DrawRectangle(0, 0, GetScreenWidth(), 40, LIGHTGRAY); // Top Gray Rectangle UI

    DrawText(TextFormat("Health: %d", playerHealth), 20, 10, 20, RED);
    DrawText(TextFormat("$ %d", playerMoney), 200, 10, 20, LIME);
    DrawText(TextFormat("Wave: %d", waveNumber), 600, 10, 20, DARKGRAY);

    Vector2 mousePos = GetMousePosition();

    for (const auto& enemy : enemies) {

        Vector2 enemyPos = enemy->getPosition();

        float hoverDistance = 15.0f;

        std::string name = enemy->getName();

        float hitbox = enemy->getRadius();

        if (name == "Slime") {
            DrawCircleV(enemyPos, hitbox, GREEN);
        } else if (name == "Knight") {
            DrawCircleV(enemyPos, hitbox, GRAY);
        } else if (name == "Fire Imp") {
            DrawCircleV(enemyPos, hitbox, ORANGE);
        } else if (name == "Spider Queen") {
            DrawCircleV(enemyPos, hitbox, BLACK);
        } else if (name == "Spiderling") {
            DrawCircleV(enemyPos, hitbox, BLACK);
        } else if (name == "Brute") {
            DrawCircleV(enemyPos, hitbox, BROWN);
        } else if (name == "Arcane Shell") {
            DrawCircleV(enemyPos, hitbox, PINK);
        } else if (name == "Flux") {
            auto fluxPtr = std::dynamic_pointer_cast<Flux>(enemy);
            if (fluxPtr) {
                Color ringColor = (fluxPtr->getShield() == "Physical") ? GRAY : PINK;
                // Draw outer ring for the shield
                DrawCircleLinesV(enemyPos, hitbox, ringColor);
                // Draw inner enemy body
                DrawCircleV(enemyPos, 12, YELLOW);
            }
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
            
            int fontSize = 14;
            int textWidth = MeasureText(info.c_str(), fontSize);
            int textX = enemyPos.x - textWidth / 2;
            int textY = barPos.y - 18;

            DrawText(info.c_str(), textX, textY, fontSize, DARKGRAY);

        }

        if (enemy->isBurning()) {
            // Position flame slightly above the enemy
            Vector2 flamePos = { enemyPos.x, enemyPos.y - 20 };

            // Optional: Flicker size with sine wave
            float flicker = 2.0f + 2.0f * sin(GetTime() * 10.0f);

            // Draw flickering fire circles
            DrawCircleV(flamePos, 6 + flicker * 0.3f, RED);
            DrawCircleV({flamePos.x + 2, flamePos.y - 4}, 4 + flicker * 0.2f, ORANGE);
            DrawCircleV({flamePos.x - 3, flamePos.y - 2}, 3 + flicker * 0.2f, YELLOW);
        }

    }

    int numSlots = 6;
    int slotSize = 80; 
    int spacing = 20;
    int startX = 20;
    int y = 600;

    int hoveredTowerIndex = -1;

    for (int i = 0; i < numSlots; i++) {
        int x = startX + i * (slotSize + spacing);

        Rectangle slotRect = { (float)x, (float)y, (float)slotSize, (float)slotSize };
        DrawRectangleRec(slotRect, DARKGRAY);

        std::string keyText = std::to_string(i + 1);
        int fontSize = 16;
        int padding = 4; // distance from the top-left corner

        DrawText(keyText.c_str(), (int)(slotRect.x + padding), (int)(slotRect.y + padding), fontSize, WHITE);

        if (CheckCollisionPointRec(mousePos, slotRect)) {
            hoveredTowerIndex = i + 1; // Save which one is hovered
            Color borderColor = (selectedTowerIndex == i + 1) ? RED : GOLD;
            DrawRectangleLinesEx(slotRect, 3, borderColor);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (costs[i+1] <= playerMoney) {
                    selectedTowerIndex = i + 1;
                    isPlacingTower = true;
                    selectedTower = nullptr;
                    HideCursor();
                } else {
                    PlaySound(SoundManager::error);
                    showNotEnoughMoney = true;
                    moneyMsgTimer = 0.0f;
                }
            }
        }
    }

    // Draw tower info at bottom right
    if (hoveredTowerIndex != -1 && !selectedTower) {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        const int padding = 12;
        const int fontSize = 20;

        std::string name = towerNames[hoveredTowerIndex];
        std::string type = towerTypes[hoveredTowerIndex];
        std::string range = towerRanges[hoveredTowerIndex];
        std::string targeting = towerTargetings[hoveredTowerIndex];
        int cost = costs[hoveredTowerIndex];

        std::string line1 = "Tower: " + name;
        std::string line2 = "Type: " + type;
        std::string line3 = "Range: " + range;
        std::string line4 = "Targeting: " + targeting;
        std::string line5 = "Cost: " + std::to_string(cost);

        float textX = (screenW / 2) + 25.0f;
        float textY = screenH - 150.0f;

        DrawText(line1.c_str(), textX, textY, fontSize, WHITE);
        DrawText(line2.c_str(), textX, textY + fontSize + padding, fontSize, WHITE);
        DrawText(line3.c_str(), textX, textY + 2 * (fontSize + padding), fontSize, WHITE);
        DrawText(line4.c_str(), textX, textY + 3 * (fontSize + padding), fontSize, WHITE);
        DrawText(line5.c_str(), textX, textY + 4 * (fontSize + padding), fontSize, WHITE);
    }

    // Pause button
    Rectangle pauseButton = {GetScreenWidth() - 100.0f, 0.0f, 40.0f, 40.0f};

    // Hover effect for pause button
    bool pauseHovered = CheckCollisionPointRec(GetMousePosition(), pauseButton);
    float scale = pauseHovered ? 1.2f : 1.0f;

    float scaledWidth = pauseButton.width * scale;
    float scaledHeight = pauseButton.height * scale;
    float offsetX = (scaledWidth - pauseButton.width) / 2.0f;
    float offsetY = (scaledHeight - pauseButton.height) / 2.0f;

    Rectangle scaledPauseButton = {
        pauseButton.x - offsetX,
        pauseButton.y - offsetY,
        scaledWidth,
        scaledHeight
    };

    // Detect button click
    if ((IsKeyPressed(KEY_P)) || (CheckCollisionPointRec(GetMousePosition(), pauseButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
        Paused = !Paused;
    }

    if (!Paused) {
        // Draw pause icon (two vertical bars)
        float barWidth = 5.0f * scale;
        float gap = 10.0f * scale;

        DrawRectangle(scaledPauseButton.x + gap, scaledPauseButton.y + 6 * scale, barWidth, scaledPauseButton.height - 12 * scale, DARKGRAY);
        DrawRectangle(scaledPauseButton.x + gap + barWidth + gap, scaledPauseButton.y + 6 * scale, barWidth, scaledPauseButton.height - 12 * scale, DARKGRAY);
    } else {
        
        Vector2 center = { scaledPauseButton.x + scaledPauseButton.width / 2.0f, scaledPauseButton.y + scaledPauseButton.height / 2.0f };
        float size = 12.0f * scale;

        Vector2 p1 = { center.x - (size / 1.5f), center.y - size }; // top point
        Vector2 p2 = { center.x - (size / 1.5f), center.y + size }; // bottom point
        Vector2 p3 = { center.x + size, center.y };                 // right point

        DrawTriangle(p1, p2, p3, DARKGRAY);
    }

    // Home button
    Rectangle homeButton = { GetScreenWidth() - 50.0f, 0.0f, 40.0f, 40.0f };

    // Determine if hovered for home button
    bool isHovered = CheckCollisionPointRec(GetMousePosition(), homeButton);

    // Enlarge the button if hovered
    scale = isHovered ? 1.2f : 1.0f;
    scaledWidth = homeButton.width * scale;
    scaledHeight = homeButton.height * scale;
    offsetX = (scaledWidth - homeButton.width) / 2.0f;
    offsetY = (scaledHeight - homeButton.height) / 2.0f;

    Rectangle scaledButton = {
        homeButton.x - offsetX,
        homeButton.y - offsetY,
        scaledWidth,
        scaledHeight
    };

    // Roof (triangle)
    Vector2 top = { scaledButton.x + scaledButton.width / 2.0f, scaledButton.y + 5 * scale };
    Vector2 left = { scaledButton.x + 5 * scale, scaledButton.y + scaledButton.height / 2.0f };
    Vector2 right = { scaledButton.x + scaledButton.width - 5 * scale, scaledButton.y + scaledButton.height / 2.0f };
    DrawTriangle(top, left, right, DARKGRAY);

    // House base (square)
    float baseX = scaledButton.x + 10 * scale;
    float baseY = scaledButton.y + scaledButton.height / 2.0f;
    float baseWidth = scaledButton.width - 20 * scale;
    float baseHeight = scaledButton.height / 2.0f - 5 * scale;
    DrawRectangle(baseX, baseY, baseWidth, baseHeight, DARKGRAY);

    if (CheckCollisionPointRec(GetMousePosition(), homeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        HomePressed = true;
    }

    DrawLineV({640, 560}, {640, 720}, DARKGRAY);
}
