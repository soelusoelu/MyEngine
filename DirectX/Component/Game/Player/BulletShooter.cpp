#include "BulletShooter.h"
#include "PlayerMotions.h"
#include "../PlayerEnemyCommon/PlayerEnemyConnection.h"
#include "../../Engine/Camera/Camera.h"
#include "../../Engine/Collider/OBBCollider.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Collision/Collision.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectManager.h"
#include "../../../Input/Input.h"
#include "../../../System/Window.h"
#include "../../../Transform/Transform3D.h"

BulletShooter::BulletShooter()
    : Component()
    , mCamera(nullptr)
    , mAnimation(nullptr)
    , mShotRaySetter(nullptr)
    , mIsADS(false)
{
}

BulletShooter::~BulletShooter() = default;

void BulletShooter::start() {
    const auto& cam = gameObject().getGameObjectManager().find("Camera");
    mCamera = cam->componentManager().getComponent<Camera>();

    mAnimation = getComponent<SkinMeshComponent>();
}

void BulletShooter::update() {
    ads();
    if (!mIsADS) {
        return;
    }
    if (!Input::joyPad().getJoyDown(JoyCode::RightButton)) {
        return;
    }

    //カメラの中心から弾となるレイを飛ばす
    const auto center = Window::size() / 2.f;
    mShotRay = mCamera->screenToRay(center);

    mShotRaySetter->setShotRay(&mShotRay);

    //アニメーション変更
    mAnimation->changeMotion(PlayerMotions::SHOT);
    mAnimation->setLoop(false);
}

void BulletShooter::setConnector(const GameObject& connector) {
    auto pec = connector.componentManager().getComponent<PlayerEnemyConnection>();
    mShotRaySetter = pec.get();
}

void BulletShooter::ads() {
    const auto& pad = Input::joyPad();
    static constexpr auto code = JoyCode::LeftButton;
    if (pad.getJoyDown(code)) {
        mIsADS = true;
        mAnimation->changeMotion(PlayerMotions::START_ADS);
        mAnimation->setLoop(false);
        mOnStartAds();
    }
    if (pad.getJoyUp(code)) {
        mIsADS = false;
        mAnimation->changeMotion(PlayerMotions::STOP_ADS);
        mAnimation->setLoop(false);
        mOnStopAds();
    }
    if (pad.getJoy(code)) {
        mOnAds();
    }
}

bool BulletShooter::isAds() const {
    return mIsADS;
}

void BulletShooter::onAds(const std::function<void()>& f) {
    mOnAds += f;
}

void BulletShooter::onStartAds(const std::function<void()>& f) {
    mOnStartAds += f;
}

void BulletShooter::onStopAds(const std::function<void()>& f) {
    mOnStopAds += f;
}
