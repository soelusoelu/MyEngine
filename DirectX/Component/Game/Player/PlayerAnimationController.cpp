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

    mAnimation->setMotionName("SecondAttack", 0);
    mAnimation->setMotionName("nazo", 1);
    mAnimation->setMotionName("nazo2", 2);
    mAnimation->setMotionName("FirstAttack", 3);
    mAnimation->setMotionName("Death", 4);
    mAnimation->setMotionName("Idol", 5);
    mAnimation->setMotionName("Roll", 6);
    mAnimation->setMotionName("Dash", 7);
    mAnimation->setMotionName("Walk", 8);
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
