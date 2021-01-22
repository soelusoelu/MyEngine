#include "EngineCamera.h"
#include "SimpleCamera.h"
#include "../../Input/Input.h"

EngineCamera::EngineCamera()
    : mCamera(std::make_unique<SimpleCamera>())
    , mCameraRotation(Quaternion::identity)
{
}

EngineCamera::~EngineCamera() = default;

void EngineCamera::update() {
    const auto& mouse = Input::mouse();
    moveCamera(mouse);
    rotateLookAtPoint(mouse);
}

SimpleCamera& EngineCamera::getCamera() const {
    return *mCamera;
}

void EngineCamera::moveCamera(const IMouse& mouse) {

}

void EngineCamera::rotateLookAtPoint(const IMouse& mouse) {
    Vector2 mouseMoveAmount;
    if (!rotateConditions(mouseMoveAmount, mouse)) {
        return;
    }

    //マウス移動量から回転軸を求める
    auto rotAxis = Vector3::normalize(Vector3(mouseMoveAmount.y, mouseMoveAmount.x, 0.f));
    //回転軸とマウス移動量から共役クォータニオンを求める
    Quaternion r(rotAxis, mouseMoveAmount.length() * 0.5f);

    mCameraRotation = Quaternion::concatenate(mCameraRotation, r);

    //カメラから注視点に向かうベクトルを求める
    auto cameraToLookAt = mCamera->getLookAt() - mCamera->getPosition();
    //求めたベクトルを正規化する
    auto normalizeCameraToLookAt = Vector3::normalize(cameraToLookAt);
    //ベクトルの長さを求める
    auto lengthCameraToLookAt = cameraToLookAt.length();
    //視線方向に回転したクォータニオンを取得する
    auto rot = Quaternion::lookRotation(normalizeCameraToLookAt);
    //視線の逆向きベクトルを求める
    auto back = Vector3::transform(Vector3::back, mCameraRotation);

    mCamera->setPosition(mCamera->getLookAt() + back * lengthCameraToLookAt);
}

bool EngineCamera::rotateConditions(Vector2& mouseMoveAmount, const IMouse& mouse) const {
    if (!mouse.getMouseButton(MouseCode::RightButton)) {
        return false;
    }

    //1フレームのマウス移動量を取得
    auto amount = mouse.getMouseMoveAmount();

    //マウスが移動していないなら終了
    if (Vector2::equal(amount, Vector2::zero)) {
        return false;
    }

    mouseMoveAmount = amount;
    return true;
}
