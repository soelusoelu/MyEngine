#pragma once

#include "../../Component.h"
#include <memory>

class GameObject;
class SpriteComponent;
class HitPoint;

class BossEnemyHitPointGauge
    : public Component
{
public:
    BossEnemyHitPointGauge();
    ~BossEnemyHitPointGauge();
    virtual void start() override;

private:
    BossEnemyHitPointGauge(const BossEnemyHitPointGauge&) = delete;
    BossEnemyHitPointGauge& operator=(const BossEnemyHitPointGauge&) = delete;

    void onSetBossEnemy(const GameObject& boss);
    void onChangeHP(const HitPoint& hp);

private:
    std::shared_ptr<SpriteComponent> mSprite;
};
