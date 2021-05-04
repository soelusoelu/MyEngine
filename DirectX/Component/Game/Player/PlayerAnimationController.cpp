#include "PlayerAnimationController.h"
#include "PlayerAttack.h"
#include "PlayerGuard.h"
#include "PlayerMove.h"
#include "PlayerRoll.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Input/Input.h"

PlayerAnimationController::PlayerAnimationController(GameObject& gameObject)
    : Component(gameObject)
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
    //int keyNum = Input::keyboard().getNumber();

    //if (keyNum == -1) {
    //    return;
    //}

    //int motinoCount = mAnimation->getMotionCount();
    //if (keyNum < mAnimation->getMotionCount()) {
    //    mAnimation->changeMotion(keyNum);
    //}

    if (mMove->isWalking()) {
        mRoll->originalUpdate();
    }
    if (!mRoll->isRolling()) {
        mMove->originalUpdate();
    }
}
