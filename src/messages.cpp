#include "messages.h"

void MessageManager::addMessage(const std::string& text, float duration) {
    messageQueue.emplace(text, duration);
}

void MessageManager::update(float deltaTime) {
    if (currentMessage) {
        currentMessage->timeLeft -= deltaTime;
        if (currentMessage->timeLeft <= 0.0f) {
            currentMessage.reset();
        }
    }

    if (!currentMessage && !messageQueue.empty()) {
        currentMessage = messageQueue.front();
        messageQueue.pop();
    }
}

void MessageManager::clear() {
    while (!messageQueue.empty()) {
        messageQueue.pop();
    }
    currentMessage.reset();
}

void MessageManager::draw() const {
    if (currentMessage) {
        int fontSize = 20;
        int screenWidth = GetScreenWidth();
        int textWidth = MeasureText(currentMessage->text.c_str(), fontSize);
        DrawText(currentMessage->text.c_str(), (screenWidth - textWidth) / 2, 10, fontSize, DARKGRAY);
    }
}

bool MessageManager::isDisplayingMessage() const {
    return currentMessage.has_value();
}

void WaveScriptManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    json data;
    file >> data;

    for (const auto& entry : data) {
        int wave = entry["wave"];
        WaveMessage msg;
        if (entry.contains("pre")) msg.pre = entry["pre"];
        if (entry.contains("post")) msg.post = entry["post"];
        if (entry.contains("warning")) msg.warning = entry["warning"];
        waveMessages[wave] = msg;
    }
}

WaveMessage WaveScriptManager::getMessagesForWave(int wave) const {
    if (waveMessages.count(wave))
        return waveMessages.at(wave);
    return WaveMessage{};
}
