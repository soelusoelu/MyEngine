#pragma once

#include "../../Component.h"
#include <memory>

class SpriteComponent;
class Stamina;

class StaminaGauge
    : public Component
{
public:
    StaminaGauge();
    ~StaminaGauge();
    virtual void start() override;

private:
    StaminaGauge(const StaminaGauge&) = delete;
    StaminaGauge& operator=(const StaminaGauge&) = delete;

    void onSetPlayer(const GameObject& player);
    void onChangeStamina(const Stamina& stamina);

private:
    std::shared_ptr<SpriteComponent> mSprite;
};
