#include "BossEnemyHitPointGauge.h"
#include "BossEnemyUIManager.h"
#include "../PlayerEnemyCommon/HitPoint.h"
#include "../../Engine/Sprite/SpriteComponent.h"
#include "../../../GameObject/GameObject.h"

BossEnemyHitPointGauge::BossEnemyHitPointGauge()
    : Component()
    , mSprite(nullptr)
{
}

BossEnemyHitPointGauge::~BossEnemyHitPointGauge() = default;

void BossEnemyHitPointGauge::start() {
    mSprite = getComponent<SpriteComponent>();

    getComponent<BossEnemyUIManager>()->callbackSetBoss(
        [&](const GameObject& boss) { onSetBossEnemy(boss); }
    );
}

void BossEnemyHitPointGauge::onSetBossEnemy(const GameObject& boss) {
    boss.componentManager().getComponent<HitPoint>()->callbackChangeHp(
        [&](const HitPoint& hp) { onChangeHP(hp); }
    );
}

void BossEnemyHitPointGauge::onChangeHP(const HitPoint& hp) {
    mSprite->setUV(0.f, 0.f, hp.getHpRate(), 1.f);
}
