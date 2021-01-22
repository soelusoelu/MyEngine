#include "EngineCamera.h"
#include "SimpleCamera.h"
#include "../../Input/Input.h"

EngineCamera::EngineCamera()
    : mCamera(std::make_unique<SimpleCamera>())
    , mCameraRotation(Quaternion::identity)
    , mLengthCameraToLookAt(0.f)
{
}

EngineCamera::~EngineCamera() = default;

void EngineCamera::update() {
    const auto& mouse = Input::mouse();

    //1フレームのマウス移動量を取得
    const auto& amount = mouse.getMouseMoveAmount();

    moveCamera(mouse, amount);
    rotateLookAtPoint(mouse, amount);
}

SimpleCamera& EngineCamera::getCamera() const {
    return *mCamera;
}

void EngineCamera::moveCamera(const IMouse& mouse, const Vector2& mouseMoveAmount) {
    if (!canMove(mouse, mouseMoveAmount)) {
        return;
    }

    //カメラの回転をもとに移動量を求める
    auto moveAmount = Vector3::transform(Vector3::right, mCameraRotation) * -mouseMoveAmount.x;
    moveAmount += Vector3::transform(Vector3::up, mCameraRotation) * mouseMoveAmount.y;
    //移動速度を決定する
    moveAmount *= MOVE_SPEED;

    //注視点と位置を設定する
    mCamera->lookAt(mCamera->getLookAt() + moveAmount);
    mCamera->setPosition(mCamera->getPosition() + moveAmount);
}

void EngineCamera::rotateLookAtPoint(const IMouse& mouse, const Vector2& mouseMoveAmount) {
    if (!canRotate(mouse, mouseMoveAmount)) {
        return;
    }

    //新しい回転軸を計算する
    computeRotation(mouseMoveAmount);

    //視線の逆向きベクトルを求める
    auto back = Vector3::transform(Vector3::back, mCameraRotation);
    //カメラから注視点までの距離を計算する
    computeLengthCameraToLookAt();

    mCamera->setPosition(mCamera->getLookAt() + back * mLengthCameraToLookAt);
}

void EngineCamera::computeRotation(const Vector2& mouseMoveAmount) {
    //マウス移動量から回転軸を求める
    auto rotAxis = Vector3::normalize(Vector3(mouseMoveAmount.y, mouseMoveAmount.x, 0.f));
    //回転軸とマウス移動量からクォータニオンを求める
    Quaternion r(rotAxis, mouseMoveAmount.length() * ROTATE_SPEED);

    mCameraRotation = Quaternion::concatenate(mCameraRotation, r);
}

void EngineCamera::zoomCamera(const IMouse& mouse) {

}

void EngineCamera::zoomIn(const IMouse& mouse) {

}

void EngineCamera::zoomOut(const IMouse& mouse) {

}

Vector3 EngineCamera::getCameraToLookAt() const {
    return (mCamera->getLookAt() - mCamera->getPosition());
}

void EngineCamera::computeLengthCameraToLookAt() {
    mLengthCameraToLookAt = getCameraToLookAt().length();
}

bool EngineCamera::canMove(const IMouse& mouse, const Vector2& mouseMoveAmount) const {
    if (!mouse.getMouseButton(MouseCode::WheelButton)) {
        return false;
    }
    if (!isMoveMousePosition(mouseMoveAmount)) {
        return false;
    }

    return true;
}

bool EngineCamera::canRotate(const IMouse& mouse, const Vector2& mouseMoveAmount) const {
    if (!mouse.getMouseButton(MouseCode::RightButton)) {
        return false;
    }
    if (!isMoveMousePosition(mouseMoveAmount)) {
        return false;
    }

    return true;
}

bool EngineCamera::isMoveMousePosition(const Vector2& mouseMoveAmount) const {
    return !Vector2::equal(mouseMoveAmount, Vector2::zero);
}
