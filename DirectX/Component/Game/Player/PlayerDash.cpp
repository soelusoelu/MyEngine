#include "PlayerDash.h"
#include "PlayerMotions.h"
#include "PlayerMove.h"
#include "PlayerWalk.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"

PlayerDash::PlayerDash()
    : Component()
    , mAnimation(nullptr)
    , mDashSpeed(0.f)
    , mIsDashing(false)
{
}

PlayerDash::~PlayerDash() = default;

void PlayerDash::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mAnimation->callbackChangeMotion([&] { onChangeMotion(); });

    getComponent<PlayerWalk>()->callbackToWalk([&] { mIsDashing = false; });
    getComponent<PlayerMove>()->callbackToStop([&] { mIsDashing = false; });
}

void PlayerDash::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mDashSpeed, "dashSpeed", inObj, alloc, mode);
}

void PlayerDash::dash(IPlayerMove& playerMove) {
    playerMove.move(mDashSpeed);
    playerMove.rotateToMoveDirection();

    if (!mIsDashing) {
        mAnimation->changeMotion(PlayerMotions::DASH);
        mIsDashing = true;

        mCallbackToDash();
    }
}

bool PlayerDash::isDashing() const {
    return mIsDashing;
}

void PlayerDash::callbackToDash(const std::function<void()>& callback) {
    mCallbackToDash += callback;
}

void PlayerDash::callbackRunOutOfStamina(const std::function<void()>& callback) {
    mCallbackRunOutOfStamina += callback;
}

void PlayerDash::onChangeMotion() {
    if (mAnimation->getCurrentMotionNumber() != PlayerMotions::DASH) {
        mIsDashing = false;
    }
}
