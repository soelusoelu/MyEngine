﻿#include "PlayerColliderController.h"
#include "../../Engine/Collider/AABBAnimationCollider.h"

PlayerColliderController::PlayerColliderController(GameObject& gameObject)
    : Component(gameObject)
    , mCollider(nullptr)
{
}

PlayerColliderController::~PlayerColliderController() = default;

void PlayerColliderController::start() {
    mCollider = getComponent<AABBAnimationCollider>();
    //mCollider->concatenate(HEAD, NOSE);
    mCollider->concatenate(HEAD, LEFT_EYE);
    mCollider->concatenate(HEAD, RIGHT_EYE);
}
