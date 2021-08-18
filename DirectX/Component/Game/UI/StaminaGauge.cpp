#include "StaminaGauge.h"
#include "PlayerUIManager.h"
#include "../Player/Stamina.h"
#include "../../Engine/Sprite/SpriteComponent.h"
#include "../../../Utility/LevelLoader.h"

StaminaGauge::StaminaGauge()
    : Component()
    , mSprite(nullptr)
{
}

StaminaGauge::~StaminaGauge() = default;

void StaminaGauge::start() {
    mSprite = getComponents<SpriteComponent>()[PlayerUIManager::SPRITE_STAMINA];

    getComponent<PlayerUIManager>()->callbackSetPlayer(
        [&](const GameObject& player) { onSetPlayer(player); }
    );
}

void StaminaGauge::onSetPlayer(const GameObject& player) {
    player.componentManager().getComponent<Stamina>()->callbackChangeStamina(
        [&](const Stamina& stamina) { onChangeStamina(stamina); }
    );
}

void StaminaGauge::onChangeStamina(const Stamina& stamina) {
    mSprite->setUV(0.f, 0.f, stamina.getStaminaRate(), 1.f);
}
