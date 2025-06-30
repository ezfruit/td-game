#include <fstream>
#include <unordered_map>
#include "json.hpp"
#include <queue>
#include "raylib.h"

using json = nlohmann::json;

struct GameMessage {
    std::string text;
    float duration;
    float timeLeft;

    GameMessage(const std::string& t, float d) : text(t), duration(d), timeLeft(d) {}
};

class MessageManager {
    private:
        std::queue<GameMessage> messageQueue;
        std::optional<GameMessage> currentMessage;

    public:
        void addMessage(const std::string& text, float duration);

        void update(float deltaTime);

        void clear();

        void draw() const;

        bool isDisplayingMessage() const;
};

struct WaveMessage {
    std::string pre;
    std::string post;
    std::string warning;
};

class WaveScriptManager {
    private:
        std::unordered_map<int, WaveMessage> waveMessages;

    public:
        void loadFromFile(const std::string& filename);

        WaveMessage getMessagesForWave(int wave) const;
};
