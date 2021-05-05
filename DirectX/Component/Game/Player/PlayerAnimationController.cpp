#include "PlayerAnimationController.h"
#include "PlayerAttack.h"
#include "PlayerGuard.h"
#include "PlayerMotions.h"
#include "PlayerMove.h"
#include "PlayerRoll.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"

PlayerAnimationController::PlayerAnimationController()
    : Component()
    , mAnimation(nullptr)
    , mMove(nullptr)
    , mRoll(nullptr)
    , mAttack(nullptr)
    , mGuard(nullptr)
{
}

PlayerAnimationController::~PlayerAnimationController() = default;

void PlayerAnimationController::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mAnimation->changeMotion(PlayerMotions::IDOL);

    mMove = getComponent<PlayerMove>();
    mRoll = getComponent<PlayerRoll>();
    mAttack = getComponent<PlayerAttack>();
    mGuard = getComponent<PlayerGuard>();
}

void PlayerAnimationController::update() {
    if (canRoll()) {
        mRoll->originalUpdate();
    }
    if (canAttack()) {
        mAttack->originalUpdate();
    }
    if (canGuard()) {
        mGuard->originalUpdate();
    }
    if (canMove()) {
        mMove->originalUpdate();
    }
}

bool PlayerAnimationController::canMove() const {
    if (mRoll->isRolling()) {
        return false;
    }
    if (mAttack->isAttacking()) {
        return false;
    }
    if (mGuard->isGuarding()) {
        return false;
    }

    return true;
}

bool PlayerAnimationController::canRoll() const {
    return mMove->isWalking();
}

bool PlayerAnimationController::canAttack() const {
    if (mRoll->isRolling()) {
        return false;
    }
    if (mGuard->isGuarding()) {
        return false;
    }

    return true;
}

bool PlayerAnimationController::canGuard() const {
    if (mRoll->isRolling()) {
        return false;
    }
    if (mAttack->isAttacking()) {
        return false;
    }

    return true;
}
