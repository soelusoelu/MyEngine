#pragma once

#include "../../Component.h"
#include <memory>

class SkinMeshComponent;

class PlayerAnimationController
    : public Component
{
public:
    PlayerAnimationController(GameObject& gameObject);
    ~PlayerAnimationController();
    virtual void start() override;
    virtual void update() override;

private:
    PlayerAnimationController(const PlayerAnimationController&) = delete;
    PlayerAnimationController& operator=(const PlayerAnimationController&) = delete;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
};

namespace PlayerMotions {
constexpr unsigned FIRST_ATTACK = 3;
constexpr unsigned SECOND_ATTACK = 0;
constexpr unsigned DEATH = 4;
constexpr unsigned IDOL = 5;
constexpr unsigned ROLL = 6;
constexpr unsigned DASH = 7;
constexpr unsigned WALK = 8;
}
