#pragma once

#include "../../Component.h"
#include "../../../Device/Function.h"
#include <functional>
#include <memory>

class GameObject;

class BossEnemyUIManager
    : public Component
{
    using Boss = std::shared_ptr<GameObject>;

public:
    BossEnemyUIManager();
    ~BossEnemyUIManager();
    void setBoss(const Boss& boss);
    void callbackSetBoss(const std::function<void(const GameObject&)>& f);

private:
    BossEnemyUIManager(const BossEnemyUIManager&) = delete;
    BossEnemyUIManager& operator=(const BossEnemyUIManager&) = delete;

private:
    Function<void(const GameObject&)> mCallbackSetBoss;
};
