#include "PlayerAttack.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Input/Input.h"

PlayerAttack::PlayerAttack(GameObject& gameObject)
    : Component(gameObject)
    , mAnimation(nullptr)
{
}

PlayerAttack::~PlayerAttack() = default;

void PlayerAttack::start() {
    mAnimation = getComponent<SkinMeshComponent>();
}

void PlayerAttack::update() {
    if (Input::joyPad().getJoyDown(JoyCode::RightButton)) {
        mAnimation->changeMotion("FirstAttack");
        mAnimation->setLoop(false);
    }
}
