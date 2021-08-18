#include "BossEnemyUIManager.h"

BossEnemyUIManager::BossEnemyUIManager()
    : Component()
{
}

BossEnemyUIManager::~BossEnemyUIManager() = default;

void BossEnemyUIManager::setBoss(const Boss& boss) {
    mCallbackSetBoss(*boss);
}

void BossEnemyUIManager::callbackSetBoss(const std::function<void(const GameObject&)>& f) {
    mCallbackSetBoss += f;
}
