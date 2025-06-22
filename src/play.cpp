#include "raylib.h"
#include "play.h"
#include "tower.h"
#include "wave.h"
#include "projectile.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include "sounds.h"
#include "images.h"
#include "messages.h"


// TODO: When placing a tower down, make it so that your cursor is shown on the UI portion not on the field.
// TODO: Make the track an actual track
// TODO: Animations for towers (archer done)
// TODO: Animations for enemies (slime done)
// TODO: Add a cooldown between starting the game and wave 1 (10 seconds maybe), also add a visual indicator where enemies will come from
// TODO: Add messages between rounds (how much income is earned) and any warnings for new/existing enemies
// TODO: Add a brief description to towers when hovering over their icons (to let players know strengths)
// TODO: Add upgrade information as well as an image of the upgrade icon
// TODO: Add a track selector (up to 3)

std::vector<Vector2> trackPoints;

std::vector<std::shared_ptr<Tower>> towers;
std::vector<std::shared_ptr<Enemy>> enemies;
std::vector<std::shared_ptr<Projectile>> projectiles;

WaveScriptManager waveScript;
MessageManager messageManager;

static int waveNumber = 0;

int playerMoney;

static int playerHealth = 100;
static int income;
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
static bool waitingToStartNextWave = false;

static Tower* selectedTower = nullptr;

std::vector<std::string> towerNames = {
    "None",       
    "Archer",     
    "Mage",      
    "Torcher",
    "Stormshaper",
    "War Drummer",
    "Gold Mine"
};

std::vector<std::string> towerTypes = {
    "None",
    "Physical",
    "Magic",
    "Fire",
    "Air",
    "None",
    "None"
};

std::vector<std::string> towerRanges = {
    "None",
    "Medium",
    "Low",
    "Very Low",
    "Very High",
    "High",
    "None"
};

std::vector<std::string> towerTargetings = {
    "None",
    "Pierce",
    "Area of Effect",
    "Single",
    "Area of Effect",
    "Towers",
    "Utility"
};

std::unordered_map<int, int> costs = {
    {1, 200},
    {2, 400},
    {3, 700},
    {4, 3000},
    {5, 1000},
    {6, 300}
};

std::unordered_map<std::string, std::vector<int>> upgradeCosts = {
    {"Archer", {250, 600, 2500, 4500, 0}},
    {"Mage", {300, 800, 3000, 6000, 0}},
    {"Torcher", {300, 1200, 4000, 10000, 0}},
    {"Stormshaper", {600, 1800, 6000, 15000, 0}},
    {"War Drummer", {750, 2500, 4500, 12000, 0}},
    {"Gold Mine", {600, 1500, 3000, 6000, 0}}
};

TargetMode currentTargetMode = FIRST;

const char* targetModeNames[] = { "First", "Last", "Strong", "Weak" };

std::vector<Explosion> explosions;
std::vector<LightningBolt> lightningBolts;
std::vector<FloatingText> floatingTexts;

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
    playerMoney = 600000;
    playerHealth = 100;
    income = 400;
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
    projectiles.clear();

    waveScript.loadFromFile("src/wavescript.json");
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

// Logic to start the next wave
void StartNextWave() {

    for (auto& tower : towers) {
        if (auto goldGen = std::dynamic_pointer_cast<Gold_Mine>(tower)) {
            goldGen->generate(playerMoney);
        }
    }

    waveNumber++;
    waveInProgress = true;
    waveCooldownTimer = 0.0f;

    spawning = true;

    std::cout << "Wave " << waveNumber << " started!\n";
}

std::string replacePlaceholders(const std::string& message, const std::unordered_map<std::string, std::string>& values) {
    std::string result = message;
    for (const auto& [key, val] : values) {
        std::string token = "${" + key + "}";
        size_t pos = result.find(token);
        while (pos != std::string::npos) {
            result.replace(pos, token.length(), val);
            pos = result.find(token, pos + val.length());
        }
    }
    return result;
}

void EndWave() {

    waitingToStartNextWave = true; // Set flag to wait

    WaveMessage msg = waveScript.getMessagesForWave(waveNumber);

    std::unordered_map<std::string, std::string> vars = {
        { "income", std::to_string(income) }
    };

    //Use for the future for placeholder text
    //std::string formatted = replacePlaceholders(msg.post, vars);

    if (waveNumber == 0) return;

    std::string incomeText = "Wave " + std::to_string(waveNumber) + " over. You earned $" + std::to_string(income) + ".";
    messageManager.addMessage(incomeText, 3.0f);
    playerMoney += income;
    income += 50;

    if (!msg.pre.empty()) {
        messageManager.addMessage(msg.pre, 3.0f);
    }
    if (!msg.post.empty()) {
        messageManager.addMessage(msg.post, 3.0f);
    }
    if (!msg.warning.empty()) {
        messageManager.addMessage(msg.warning, 3.0f);
    }
}

// Game logic updates
void UpdatePlaying() {

    bool allDefeated = true;
    float deltaTime = GetFrameTime();

    Vector2 goal = trackPoints.back();

    for (auto& tower : towers) {
        if (tower->getName() != "War Drummer") {
            tower->resetBuffs();  // Reset multipliers
        }
    }

    for (auto& tower : towers) {
        if (auto drummer = std::dynamic_pointer_cast<War_Drummer>(tower)) {
            drummer->update(deltaTime, enemies, trackPoints, projectiles); // Apply War Drummer buffs
        }
    }

    for (auto& tower : towers) {
        tower->update(deltaTime, enemies, trackPoints, projectiles); // Let the towers attack
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
                    projectile->ApplyAOEDamage(*projectile, projectile->getPosition(), projectile->getAOERadius(), projectile->getDamage(), projectile->getDamageType());
                    explosions.push_back({projectile->getPosition(), projectile->getAOERadius()});
                    projectile->deactivate();  // deactivate projectile because it exploded
                } else {
                    int prevHealth = enemy->getHealth();
                    enemy->takeDamage(projectile->getDamage(), projectile->getDamageType(), projectile->getDamageTargeting());
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
        // Add new enemies after the loop
        enemies.insert(enemies.end(), std::make_move_iterator(deathSpawns.begin()), std::make_move_iterator(deathSpawns.end()));
        deathSpawns.clear();
    }

    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](const std::shared_ptr<Projectile>& p) { return !p->isActive(); }), projectiles.end());

    for (auto& exp : explosions) {
        exp.timeAlive += deltaTime;
        if (exp.timeAlive >= exp.duration) {
            exp.active = false;
        }
    }

    explosions.erase(std::remove_if(explosions.begin(), explosions.end(), [](const Explosion& e) { return !e.active; }), explosions.end());

    for (auto& lightning : lightningBolts) {
        lightning.timeAlive += deltaTime;
        if (lightning.timeAlive >= lightning.duration) {
            lightning.active = false;
        }
    }

    lightningBolts.erase(std::remove_if(lightningBolts.begin(), lightningBolts.end(), [](const LightningBolt& l) { return !l.active; }), lightningBolts.end());

    for (auto& text : floatingTexts) {
        text.timeAlive += deltaTime;
        text.position.y -= text.riseSpeed * deltaTime;
    }
    
    floatingTexts.erase(std::remove_if(floatingTexts.begin(), floatingTexts.end(), [](const FloatingText& t) { return t.timeAlive >= t.duration; }), floatingTexts.end());

    for (int i = enemies.size() - 1; i >= 0; --i) {
        Vector2 pos = enemies[i]->getPosition();
        float distanceToGoal = Vector2Distance(pos, goal);

        if (distanceToGoal < 5.0f) {
            enemies[i]->unloadFrames(); // Unload the texture when they get to the end
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
            enemies[i]->unloadFrames();
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

    if (!waveInProgress && !waitingToStartNextWave) {
        if (waveCooldownTimer >= waveCooldown) {
            EndWave();
        }
    } else if (!spawning && !waitingToStartNextWave) {
        if (waveCooldownTimer >= waveDuration) {
            EndWave();
        }
    }

    messageManager.update(GetFrameTime());

    if (waitingToStartNextWave && !messageManager.isDisplayingMessage()) {
        StartNextWave();
        waitingToStartNextWave = false;
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
                } else if (type == "Husk") {
                    enemy = std::make_shared<Husk>();
                } else if (type == "Exoskeleton") {
                    enemy = std::make_shared<Exoskeleton>();
                } else if (type == "Goliath") {
                    enemy = std::make_shared<Goliath>();
                } else if (type == "Sludge") {
                    enemy = std::make_shared<Sludge>();
                } else if (type == "Lava Golem") {
                    enemy = std::make_shared<Lava_Golem>();
                } else if (type == "Obsidian Behemoth") {
                    enemy = std::make_shared<Obsidian_Behemoth>();
                } else if (type == "Ravager") {
                    enemy = std::make_shared<Ravager>();
                } else if (type == "Arcane Warden") {
                    PlaySound(SoundManager::arcane_warden_spawn);
                    enemy = std::make_shared<Arcane_Warden>();
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

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && GetMousePosition().y > 40 && GetMousePosition().y < 560 && !isPlacingTower) {
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
                    break;
                case 2:
                    towers.push_back(std::make_shared<Mage>(mousePos));
                    playerMoney -= costs[2];
                    break;
                case 3:
                    towers.push_back(std::make_shared<Torcher>(mousePos));
                    playerMoney -= costs[3];
                    break;
                case 4:
                    towers.push_back(std::make_shared<Stormshaper>(mousePos));
                    playerMoney -= costs[4];
                    break;
                case 5:
                    towers.push_back(std::make_shared<War_Drummer>(mousePos));
                    playerMoney -= costs[5];
                    break;
                case 6:
                    towers.push_back(std::make_shared<Gold_Mine>(mousePos));
                    playerMoney -= costs[6];
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
        previewPosition = GetMousePosition();
        HideCursor();

        int range = 0;
        Texture2D previewIcon;
        float desiredSize = 40.0f;

        switch (selectedTowerIndex) {
            case 1: range = 150; previewIcon = ImageHandler::archerIcon; break;
            case 2: range = 100; previewIcon = ImageHandler::mageIcon; break;
            case 3: range = 75;  previewIcon = ImageHandler::torcherIcon; break;
            case 4: range = 300; previewIcon = ImageHandler::stormshaperIcon; break;
            case 5: range = 200; previewIcon = ImageHandler::wardrummerIcon; break;
            case 6: range = 50;  previewIcon = ImageHandler::goldmineIcon; break;
        }

        bool invalidPlacement = IsOnTrack(previewPosition, trackPoints) || IsOnTower(previewPosition, towers) || !IsWithinBounds(previewPosition);
        Color color = invalidPlacement ? Fade(RED, 0.3f) : Fade(BLUE, 0.3f);
        DrawCircle(previewPosition.x, previewPosition.y, range, color);

        // Draw the icon centered at the mouse
        float scale = desiredSize / previewIcon.width;
        float iconWidth = previewIcon.width * scale;
        float iconHeight = previewIcon.height * scale;

        Vector2 iconPos = {
            previewPosition.x - iconWidth / 2.0f,
            previewPosition.y - iconHeight / 2.0f
        };

        DrawTextureEx(previewIcon, iconPos, 0.0f, scale, WHITE);
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
        tower->draw();
    }

    if (isPlacingTower) {

        showNotEnoughMoney = false;

        std::string tower;

        switch (selectedTowerIndex) {
            case 1: tower = "Archer"; break; // Archer
            case 2: tower = "Mage"; break; // Mage
            case 3: tower = "Torcher"; break; // Torcher
            case 4: tower = "Stormshaper"; break; // Stormshaper
            case 5: tower = "War Drummer"; break; // War Drummer
            case 6: tower = "Gold Mine"; break; // Gold Mine
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

    Vector2 mousePos = GetMousePosition();

    for (const auto& enemy : enemies) {

        Vector2 enemyPos = enemy->getPosition();

        float hoverDistance = 15.0f;

        enemy->draw();

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

    for (auto& proj : projectiles) {
        proj->draw();
    }

    for (auto& exp : explosions) {
        float alpha = 1.0f - (exp.timeAlive / exp.duration); // fade out
        alpha = Clamp(alpha, 0.0f, 1.0f);
        Color color = Fade(ORANGE, alpha);
        DrawCircleV(exp.position, exp.radius, color);
    }

    for (auto& lightning : lightningBolts) {
        float alpha = 1.0f - (lightning.timeAlive / lightning.duration);
        alpha = Clamp(alpha, 0.0f, 1.0f);
        Color color = Fade(lightning.color, alpha);
        Vector2 prev = lightning.start;
        for (auto& point : lightning.points) {
            DrawLineV(prev, point, color);
            prev = point;
        }
        DrawLineV(prev, lightning.end, color);
    }

    for (auto& text : floatingTexts) {
        float alpha = 1.0f - (text.timeAlive / text.duration);
        Color faded = Fade(text.color, alpha);

        int fontSize = 18;
        int textWidth = MeasureText(text.text.c_str(), fontSize);

        Vector2 drawPos = {
            text.position.x - textWidth / 2.0f,
            text.position.y - fontSize / 2.0f  // optional: vertically center
        };

        DrawText(text.text.c_str(), (int)drawPos.x, (int)drawPos.y, fontSize, faded);
    }

    DrawRectangle(0, 0, GetScreenWidth(), 40, LIGHTGRAY); // Top Gray Rectangle UI

    messageManager.draw();

    DrawText(TextFormat("Health: %d", playerHealth), 20, 10, 20, RED);
    DrawText(TextFormat("$ %d", playerMoney), 200, 10, 20, LIME);
    if (!messageManager.isDisplayingMessage()) {
        DrawText(TextFormat("Wave: %d", waveNumber), 600, 10, 20, DARKGRAY);
    }

    DrawRectangle(0, 560, GetScreenWidth(), 160, LIGHTGRAY); // Bottom Gray Rectangle UI

    // When a tower is selected (clicked on in the field)
    if (selectedTower) {

        if (selectedTower->getWarDrummerBuff()) {
            Vector2 pos = selectedTower->getPosition();
            Texture2D icon = ImageHandler::wardrummerIcon;

            float scale = 1.5f;
            float iconWidth = icon.width * scale;
            float iconHeight = icon.height * scale;

            // Position above the tower
            Vector2 iconPos = {
                pos.x - 12,  // center horizontally
                pos.y - iconHeight - 20 // a bit above the tower
            };

            DrawTextureEx(icon, iconPos, 0.0f, scale, WHITE);
        }

        int infoX = GetScreenWidth() / 2 + 25;
        int infoY = GetScreenHeight() - 160;
        DrawRectangle(infoX, infoY, 200, 160, LIGHTGRAY);
        DrawText(selectedTower->getName().c_str(), infoX + 10, infoY + 10, 24, DARKGRAY);
        DrawText(TextFormat("Level: %d", selectedTower->getLevel()), infoX + 10, infoY + 45, 20, DARKGRAY);
        if (auto goldGen = dynamic_cast<Gold_Mine*>(selectedTower)) {
            DrawText(TextFormat("Money Generated: %d", goldGen->getTotalMoneyGenerated()), infoX + 10, infoY + 75, 20, DARKGRAY);
        } else {
            DrawText(TextFormat("Damage Dealt: %d", selectedTower->getTotalDamageDealt()), infoX + 10, infoY + 75, 20, DARKGRAY);
        }

        Rectangle upgradeBtn = { (float)(infoX + 10), (float)(infoY + 100), 110, 50 };
        Rectangle sellBtn = { (float)(infoX + 130), (float)(infoY + 100), 110, 50 };

        DrawRectangleRec(upgradeBtn, GREEN);
        DrawRectangleRec(sellBtn, RED);

        DrawText("Upgrade (E)", upgradeBtn.x + 10, upgradeBtn.y + 8, 16, WHITE);
        DrawText("Sell (X)", sellBtn.x + 29, sellBtn.y + 8, 16, WHITE);

        int upgradeCost = upgradeCosts[selectedTower->getName()][selectedTower->getLevel() - 1];
        int fontSize = 20;

        std::string costStr = TextFormat("$%d", upgradeCost);
        int textWidth = MeasureText(costStr.c_str(), fontSize);

        // Center horizontally
        int x = upgradeBtn.x + 55 - (textWidth / 2);
        int y = GetScreenHeight() - 30;

        DrawText(costStr.c_str(), x, y, fontSize, WHITE);

        std::string sellValueStr = TextFormat("$%d", selectedTower->getValue());
        textWidth = MeasureText(sellValueStr.c_str(), fontSize);

        x = sellBtn.x + 55 - (textWidth / 2);

        DrawText(sellValueStr.c_str(), x, y, fontSize, WHITE);

        // Targeting mode button
        Rectangle targetBtn = { (float)GetScreenWidth() - 160, (float)GetScreenHeight() - 40, 150, 35 };
        DrawRectangleRec(targetBtn, DARKGRAY);

        std::string targetText = TextFormat("Targets: %s", targetModeNames[selectedTower->getTargetMode()]);
        textWidth = MeasureText(targetText.c_str(), 18);
        DrawText(targetText.c_str(), targetBtn.x + (targetBtn.width - textWidth) / 2, targetBtn.y + 10, 18, WHITE);

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
                selectedTower->unloadFrames();
                towers.erase(std::remove_if(towers.begin(), towers.end(),
                    [&](const std::shared_ptr<Tower>& t) { return t.get() == selectedTower; }),
                    towers.end());
                selectedTower = nullptr;
            }
            else if (CheckCollisionPointRec(mouse, targetBtn)) {
                currentTargetMode = static_cast<TargetMode>((selectedTower->getTargetMode() + 1) % 4);
                selectedTower->setTargetMode(currentTargetMode);
            }
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

        Texture2D icon = ImageHandler::towerIcons[i];

        Vector2 scale = { 3.0f, 3.0f };  // 3x size = 48x48 on screen

        float scaledSize = icon.width * scale.x;
        float iconX = slotRect.x + (slotSize - scaledSize) / 2.0f;
        float iconY = slotRect.y + (slotSize - scaledSize) / 2.0f;

        DrawTextureEx(icon, { iconX, iconY }, 0.0f, scale.x, WHITE);

        // Draw cost underneath the slot
        std::string costText = "$" + std::to_string(costs.at(i + 1));
        int costFontSize = 20;
        int costTextWidth = MeasureText(costText.c_str(), costFontSize);

        DrawText(
            costText.c_str(),
            (int)(slotRect.x + slotSize / 2 - costTextWidth / 2),
            (int)(slotRect.y + slotSize + 4), // a bit below the square
            costFontSize,
            LIME
        );

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
