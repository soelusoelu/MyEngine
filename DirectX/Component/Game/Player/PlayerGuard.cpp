#include "PlayerGuard.h"
#include "PlayerMotions.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Input/Input.h"

PlayerGuard::PlayerGuard()
    : Component()
    , mAnimation(nullptr)
    , mIsGuarding(false)
{
}

PlayerGuard::~PlayerGuard() = default;

void PlayerGuard::start() {
    mAnimation = getComponent<SkinMeshComponent>();
}

void PlayerGuard::update() {
    if (!mIsGuarding) {
        return;
    }

    if (Input::joyPad().getJoyUp(JoyCode::LeftButton)) {
        mAnimation->changeMotion(PlayerMotions::GUARD_END);
        mAnimation->setLoop(false);
        mIsGuarding = false;
    }
}

void PlayerGuard::originalUpdate() {
    if (Input::joyPad().getJoyDown(JoyCode::LeftButton)) {
        mAnimation->changeMotion(PlayerMotions::GUARD_START);
        mAnimation->setLoop(false);
        mIsGuarding = true;
    }
}

bool PlayerGuard::isGuarding() const {
    return mIsGuarding;
}
