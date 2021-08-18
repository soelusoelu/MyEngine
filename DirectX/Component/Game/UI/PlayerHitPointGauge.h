#pragma once

#include "../../Component.h"
#include <memory>

class GameObject;
class SpriteComponent;
class HitPoint;

class PlayerHitPointGauge
    : public Component
{
public:
    PlayerHitPointGauge();
    ~PlayerHitPointGauge();
    virtual void start() override;

private:
    PlayerHitPointGauge(const PlayerHitPointGauge&) = delete;
    PlayerHitPointGauge& operator=(const PlayerHitPointGauge&) = delete;

    void onSetPlayer(const GameObject& player);
    void onChangeHP(const HitPoint& hp);

private:
    std::shared_ptr<SpriteComponent> mSprite;
};
