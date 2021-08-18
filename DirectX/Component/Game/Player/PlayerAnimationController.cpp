#include "PlayerAnimationController.h"
#include "PlayerMotions.h"
#include "PlayerMove.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"

PlayerAnimationController::PlayerAnimationController()
    : Component()
    , mAnimation(nullptr)
    , mMove(nullptr)
{
}

PlayerAnimationController::~PlayerAnimationController() = default;

void PlayerAnimationController::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mAnimation->changeMotion(PlayerMotions::IDOL);

    mMove = getComponent<PlayerMove>();
}

void PlayerAnimationController::update() {
    mMove->originalUpdate();
}
