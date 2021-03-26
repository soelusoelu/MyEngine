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
