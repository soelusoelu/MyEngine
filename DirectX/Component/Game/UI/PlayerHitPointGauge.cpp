#include "PlayerHitPointGauge.h"
#include "PlayerUIManager.h"
#include "../PlayerEnemyCommon/HitPoint.h"
#include "../../Engine/Sprite/SpriteComponent.h"
#include "../../../GameObject/GameObject.h"

PlayerHitPointGauge::PlayerHitPointGauge()
    : Component()
    , mSprite(nullptr)
{
}

PlayerHitPointGauge::~PlayerHitPointGauge() = default;

void PlayerHitPointGauge::start() {
    mSprite = getComponents<SpriteComponent>()[PlayerUIManager::SPRITE_HP];

    getComponent<PlayerUIManager>()->callbackSetPlayer(
        [&](const GameObject& player) { onSetPlayer(player); }
    );
}

void PlayerHitPointGauge::onSetPlayer(const GameObject& player) {
    player.componentManager().getComponent<HitPoint>()->callbackChangeHp(
        [&](const HitPoint& hp) { onChangeHP(hp); }
    );
}

void PlayerHitPointGauge::onChangeHP(const HitPoint& hp) {
    mSprite->setUV(0.f, 0.f, hp.getHpRate(), 1.f);
}
