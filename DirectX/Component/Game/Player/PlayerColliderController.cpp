#include "PlayerColliderController.h"
#include "../PlayerEnemyCommon/HitPoint.h"
#include "../../Engine/Collider/Collider.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"

PlayerColliderController::PlayerColliderController()
    : Component()
    , mHP(nullptr)
{
}

PlayerColliderController::~PlayerColliderController() = default;

void PlayerColliderController::start() {
    mHP = getComponent<HitPoint>();
}

void PlayerColliderController::onCollisionEnter(Collider& other) {
    if (other.gameObject().tag() == "Enemy") {
        mHP->takeDamage(10);
        Debug::log("damege!");

        return;
    }
}
