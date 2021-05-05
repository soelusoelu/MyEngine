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
    PlayerAnimationController();
    ~PlayerAnimationController();
    virtual void start() override;
    virtual void update() override;

private:
    PlayerAnimationController(const PlayerAnimationController&) = delete;
    PlayerAnimationController& operator=(const PlayerAnimationController&) = delete;

    bool canMove() const;
    bool canRoll() const;
    bool canAttack() const;
    bool canGuard() const;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::shared_ptr<PlayerMove> mMove;
    std::shared_ptr<PlayerRoll> mRoll;
    std::shared_ptr<PlayerAttack> mAttack;
    std::shared_ptr<PlayerGuard> mGuard;
};
