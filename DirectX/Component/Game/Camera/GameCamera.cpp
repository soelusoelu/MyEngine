#include "GameCamera.h"
#include "LockOn.h"
#include "../../Engine/Camera/Camera.h"
#include "../../../Device/Time.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectManager.h"
#include "../../../Input/Input.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"

GameCamera::GameCamera()
    : Component()
    , mCamera(nullptr)
    , mPlayer(nullptr)
    , mLockOn(nullptr)
    , mUnlockOnLerpTimer(std::make_unique<Time>())
    , mRotateSpeed(0.f)
    , mToPlayerDistance(0.f)
    , mLookAtOffsetY(0.f)
    , mStartPositionY(0.f)
{
}

GameCamera::~GameCamera() = default;

void GameCamera::start() {
    const auto& cam = gameObject().getGameObjectManager().find("Camera");
    mCamera = cam->componentManager().getComponent<Camera>();

    auto lockOn = getComponent<LockOn>();
    //LockOnからILockOnへアップキャスト
    mLockOn = lockOn.get();

    mLockOn->callbackUnlockOn([&] { onUnlockOn(); });
}

void GameCamera::lateUpdate() {
    //ロックオン中ならそっちにまかせる
    if (mLockOn->isLockOn()) {
        return;
    }

    const auto& pt = mPlayer->transform();
    //注視点を求める
    auto playerUpPoint = pt.getPosition() + Vector3::up * mLookAtOffsetY;
    auto lookAt = playerUpPoint;
    //ロックオンが外れて間もないなら補間する
    if (!mUnlockOnLerpTimer->isTime()) {
        mUnlockOnLerpTimer->update();
        lookAt = Vector3::lerp(mCamera->getLookAt(), lookAt, mUnlockOnLerpTimer->rate());
    }
    mCamera->lookAt(lookAt);

    const auto& rightStick = Input::joyPad().rightStick();
    auto& t = transform();
    //スティック入力から角度を求める
    if (!Vector2::equal(rightStick, Vector2::zero)) {
        t.rotate(Vector3(rightStick.y, -rightStick.x, 0.f) * mRotateSpeed * Time::deltaTime);
    }
    //カメラ位置を求める
    auto cameraPos = playerUpPoint + -t.forward() * mToPlayerDistance;
    //ロックオンが外れて間もないなら補間する
    if (!mUnlockOnLerpTimer->isTime()) {
        cameraPos = Vector3::lerp(mCamera->getPosition(), cameraPos, mUnlockOnLerpTimer->rate());
    }

    mCamera->setPosition(cameraPos);
}

void GameCamera::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mRotateSpeed, "rotateSpeed", inObj, alloc, mode);
    JsonHelper::getSet(mToPlayerDistance, "cameraToPlayerDistance", inObj, alloc, mode);
    JsonHelper::getSet(mLookAtOffsetY, "lookAtOffsetY", inObj, alloc, mode);
    JsonHelper::getSet(mStartPositionY, "startPositionY", inObj, alloc, mode);
    mUnlockOnLerpTimer->saveAndLoad(inObj, alloc, mode);
    mUnlockOnLerpTimer->forceOverlimit();
}

void GameCamera::setPlayer(const std::shared_ptr<GameObject>& player) {
    mPlayer = player;

    const auto& pt = mPlayer->transform();
    const auto& pp = pt.getPosition();

    auto lookAt = pp + Vector3::up * mLookAtOffsetY;
    auto pos = pp + -pt.forward() * mToPlayerDistance + Vector3::up * mStartPositionY;

    calcRotation(lookAt, pos);
}

float GameCamera::getDistanceToPlayer() const {
    return mToPlayerDistance;
}

void GameCamera::calcRotation(const Vector3& lookAt, const Vector3& pos) {
    auto eye = Vector3::normalize(lookAt - pos);
    transform().setRotation(Quaternion::lookRotation(eye));
}

void GameCamera::onUnlockOn() {
    calcRotation(mCamera->getLookAt(), mCamera->getPosition());
    mUnlockOnLerpTimer->reset();
}
