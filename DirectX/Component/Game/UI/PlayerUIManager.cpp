#include "PlayerUIManager.h"

PlayerUIManager::PlayerUIManager()
    : Component()
{
}

PlayerUIManager::~PlayerUIManager() = default;

void PlayerUIManager::setPlayer(const Player& player) {
    mCallbackSetPlayer(*player);
}

void PlayerUIManager::callbackSetPlayer(const std::function<void(const GameObject&)>& callback) {
    mCallbackSetPlayer += callback;
}
