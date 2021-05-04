#pragma once

#include "../../Component.h"
#include <memory>

class SkinMeshComponent;
class PlayerMove;
class PlayerRoll;
class PlayerAttack;
class PlayerGuard;

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
    std::shared_ptr<PlayerMove> mMove;
    std::shared_ptr<PlayerRoll> mRoll;
    std::shared_ptr<PlayerAttack> mAttack;
    std::shared_ptr<PlayerGuard> mGuard;
};

namespace PlayerMotions {
constexpr unsigned FIRST_ATTACK_START = 11;
constexpr unsigned FIRST_ATTACK_END = 1;
constexpr unsigned SECOND_ATTACK_START = 0;
constexpr unsigned SECOND_ATTACK_END = 2;
constexpr unsigned GUARD_START = 3;
constexpr unsigned GUARD_END = 4;
constexpr unsigned TAKE_DAMAGE = 5;
constexpr unsigned DEATH = 6;
constexpr unsigned IDOL = 7;
constexpr unsigned ROLL = 8;
constexpr unsigned DASH = 9;
constexpr unsigned WALK = 10;
}
