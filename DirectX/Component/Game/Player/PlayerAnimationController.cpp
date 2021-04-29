#include "PlayerAnimationController.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Input/Input.h"

PlayerAnimationController::PlayerAnimationController(GameObject& gameObject)
    : Component(gameObject)
    , mAnimation(nullptr)
{
}

PlayerAnimationController::~PlayerAnimationController() = default;

void PlayerAnimationController::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    //mAnimation->tPose();

    mAnimation->setMotionName(PlayerMotionName::SECOND_ATTACK, 0);
    mAnimation->setMotionName("nazo", 1);
    mAnimation->setMotionName("nazo2", 2);
    mAnimation->setMotionName(PlayerMotionName::FIRST_ATTACK, 3);
    mAnimation->setMotionName(PlayerMotionName::DEATH, 4);
    mAnimation->setMotionName(PlayerMotionName::IDOL, 5);
    mAnimation->setMotionName(PlayerMotionName::ROLL, 6);
    mAnimation->setMotionName(PlayerMotionName::DASH, 7);
    mAnimation->setMotionName(PlayerMotionName::WALK, 8);
}

void PlayerAnimationController::update() {
    int keyNum = Input::keyboard().getNumber();

    if (keyNum == -1) {
        return;
    }

    int motinoCount = mAnimation->getMotionCount();
    if (keyNum < mAnimation->getMotionCount()) {
        mAnimation->changeMotion(keyNum);
    }
}
