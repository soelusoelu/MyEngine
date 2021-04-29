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

namespace PlayerMotionName {
constexpr char FIRST_ATTACK[] = "FirstAttack";
constexpr char SECOND_ATTACK[] = "SecondAttack";
constexpr char DEATH[] = "Death";
constexpr char IDOL[] = "Idol";
constexpr char ROLL[] = "Roll";
constexpr char DASH[] = "Dash";
constexpr char WALK[] = "Walk";
}
