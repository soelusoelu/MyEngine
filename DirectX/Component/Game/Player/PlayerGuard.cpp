#include "PlayerGuard.h"
#include "PlayerAnimationController.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Input/Input.h"

PlayerGuard::PlayerGuard(GameObject& gameObject)
    : Component(gameObject)
    , mAnimation(nullptr)
{
}

PlayerGuard::~PlayerGuard() = default;

void PlayerGuard::start() {
    mAnimation = getComponent<SkinMeshComponent>();
}

void PlayerGuard::update() {
    if (Input::joyPad().getJoyDown(JoyCode::LeftButton)) {
        mAnimation->changeMotion(PlayerMotions::GUARD_START);
        mAnimation->setLoop(false);
    }
    if (Input::joyPad().getJoyUp(JoyCode::LeftButton)) {
        mAnimation->changeMotion(PlayerMotions::GUARD_END);
        mAnimation->setLoop(false);
    }
}
