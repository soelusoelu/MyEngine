#include "PlayerWalk.h"
#include "BulletShooter.h"
#include "PlayerDash.h"
#include "PlayerMotions.h"
#include "PlayerMove.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"

PlayerWalk::PlayerWalk()
    : Component()
    , mAnimation(nullptr)
    , mBulletShooter(nullptr)
    , mCameraRotation(nullptr)
    , mWalkSpeed(0.f)
    , mIsWalking(false)
{
}

PlayerWalk::~PlayerWalk() = default;

void PlayerWalk::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mAnimation->callbackChangeMotion([&] { onChangeMotion(); });
    mBulletShooter = getComponent<BulletShooter>();
    mBulletShooter->onAds([&] { onAds(); });

    getComponent<PlayerDash>()->callbackToDash([&] { mIsWalking = false; });
    getComponent<PlayerMove>()->callbackToStop([&] { mIsWalking = false; });
}

void PlayerWalk::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mWalkSpeed, "walkSpeed", inObj, alloc, mode);
}

void PlayerWalk::walk(IPlayerMove& playerMove) {
    playerMove.move(mWalkSpeed);
    //rotate(playerMove);

    if (!mIsWalking) {
        //mAnimation->changeMotion(PlayerMotions::LEFT_MOVE_ON_ADS);
        //mAnimation->setLoop(true);
        mIsWalking = true;

        mCallbackToWalk();
    }
}

bool PlayerWalk::isWalking() const {
    return mIsWalking;
}

void PlayerWalk::setCameraRotation(const Quaternion& cameraRotation) {
    mCameraRotation = &cameraRotation;
}

void PlayerWalk::callbackToWalk(const std::function<void()>& callback) {
    mCallbackToWalk += callback;
}

void PlayerWalk::rotate(IPlayerMove& playerMove) {
    if (!mBulletShooter->isAds()) {
        playerMove.rotateToMoveDirection();
    }
}

void PlayerWalk::onChangeMotion() {
    if (mAnimation->getCurrentMotionNumber() != PlayerMotions::LEFT_MOVE_ON_ADS) {
        mIsWalking = false;
    }
}

void PlayerWalk::onAds() {
    //カメラ角度のY軸だけ考慮して回転する
    auto euler = mCameraRotation->euler();
    euler.x = 0.f;
    euler.z = 0.f;
    transform().setRotation(euler);
}
