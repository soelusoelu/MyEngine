#include "TPSCamera.h"
#include "../Player/BulletShooter.h"
#include "../../Engine/Camera/Camera.h"
#include "../../../Device/Time.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectManager.h"
#include "../../../Input/Input.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/JsonHelper.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"

TPSCamera::TPSCamera()
    : Component()
    , mCamera(nullptr)
    , mPlayer(nullptr)
    , mRotateSpeed(0.f)
    , mToPlayerDistance(0.f)
    , mPlayerHeadPointY(0.f)
    , mAdsPosition()
    , mIsInverseX(true)
    , mIsInverseY(true)
    , mIsCalcPosition(true)
{
}

TPSCamera::~TPSCamera() = default;

void TPSCamera::start() {
    const auto& cam = gameObject().getGameObjectManager().find("Camera");
    mCamera = cam->componentManager().getComponent<Camera>();
}

void TPSCamera::update() {
    const auto& pad = Input::joyPad();
    const auto rightStick = pad.rightStick();

    //スティックが動いていれば角度を計算し直す
    if (!Vector2::equal(rightStick, Vector2::zero)) {
        calcRotation(rightStick);
    }

    calcPosition();
    calcLookAt();
}

void TPSCamera::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mRotateSpeed, "rotateSpeed", inObj, alloc, mode);
    JsonHelper::getSet(mToPlayerDistance, "cameraToPlayerDistance", inObj, alloc, mode);
    JsonHelper::getSet(mPlayerHeadPointY, "playerHeadPointY", inObj, alloc, mode);
    JsonHelper::getSet(mAdsPosition, "adsPosition", inObj, alloc, mode);
}

void TPSCamera::drawInspector() {
    ImGui::Checkbox("Inverse X", &mIsInverseX);
    ImGui::Checkbox("Inverse Y", &mIsInverseY);
    ImGuiWrapper::dragFloat("To Player Distance", mToPlayerDistance, 0.1f, 0.f, FLT_MAX);
    ImGuiWrapper::dragFloat("Player Head Point Y", mPlayerHeadPointY, 0.1f, 0.f, FLT_MAX);
    ImGuiWrapper::dragVector3("ADS Position", mAdsPosition, 0.1f);
}

void TPSCamera::setPlayer(const std::shared_ptr<GameObject>& player) {
    mPlayer = player;

    auto bs = mPlayer->componentManager().getComponent<BulletShooter>();
    bs->onStartAds([&] { mIsCalcPosition = false; });
    bs->onAds([&] { onAds(); });
    bs->onStopAds([&] { mIsCalcPosition = true; });
}

void TPSCamera::calcLookAt() const {
    auto toPlayerHeadPoint = Vector3::normalize(getPlayerHeadPoint() - getCalcPosition());
    mCamera->lookAt(toPlayerHeadPoint * 1000.f);
}

void TPSCamera::calcRotation(const Vector2& rightStick) const {
    auto& t = transform();
    auto euler = t.getRotation().euler();
    const auto speed = mRotateSpeed * Time::deltaTime;

    auto rotateX = rightStick.x * speed;
    if (mIsInverseX) {
        rotateX *= -1.f;
    }
    euler.y += rotateX;

    auto rotateY = -rightStick.y * speed;
    if (mIsInverseY) {
        rotateY *= -1.f;
    }
    euler.x += rotateY;

    t.setRotation(euler);
}

void TPSCamera::calcPosition() const {
    if (mIsCalcPosition) {
        mCamera->setPosition(getCalcPosition());
    }
}

Vector3 TPSCamera::getPlayerHeadPoint() const {
    const auto& pt = mPlayer->transform();
    return pt.getPosition() + Vector3::up * mPlayerHeadPointY * pt.getScale().y;
}

Vector3 TPSCamera::getCalcPosition() const {
    return getPlayerHeadPoint() + -transform().forward() * mToPlayerDistance * mPlayer->transform().getScale().z;
}

void TPSCamera::onAds() {
    const auto& t = transform();
    Vector3 offset(
        t.right() * mAdsPosition.x +
        t.up() * mAdsPosition.y +
        t.forward() * mAdsPosition.z
    );
    mCamera->setPosition(getPlayerHeadPoint() + offset);
}
