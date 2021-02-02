#include "EngineCamera.h"
#include "SimpleCamera.h"
#include "../../Input/Input.h"

EngineCamera::EngineCamera()
    : mCamera(std::make_unique<SimpleCamera>())
    , mCameraRotation(Quaternion::identity)
    , mLengthCameraToLookAt(0.f)
    , mIsComputeLengthCameraToLookAt(true)
{
}

EngineCamera::~EngineCamera() = default;

void EngineCamera::initialize() {
    mCameraRotation = Quaternion::identity;
    mLengthCameraToLookAt = 0.f;
    mIsComputeLengthCameraToLookAt = true;
}

void EngineCamera::update() {
    const auto& mouse = Input::mouse();

    //1フレームのマウス移動量を取得
    const auto& amount = mouse.getMouseMoveAmount();

    moveCamera(mouse, amount);
    rotateLookAtPoint(mouse, amount);
    zoomCamera(mouse);
}

SimpleCamera& EngineCamera::getCamera() const {
    return *mCamera;
}

Vector3 EngineCamera::right() const {
    return Vector3::transform(Vector3::right, mCameraRotation);
}

Vector3 EngineCamera::up() const {
    return Vector3::transform(Vector3::up, mCameraRotation);
}

Vector3 EngineCamera::forward() const {
    return Vector3::transform(Vector3::forward, mCameraRotation);
}

void EngineCamera::moveCamera(const IMouse& mouse, const Vector2& mouseMoveAmount) {
    if (!canMove(mouse, mouseMoveAmount)) {
        return;
    }

    //カメラの回転をもとに移動量を求める
    auto moveAmount = right() * -mouseMoveAmount.x;
    moveAmount += up() * mouseMoveAmount.y;
    //移動速度を決定する
    moveAmount *= MOVE_SPEED;

    //注視点と位置を設定する
    mCamera->lookAt(mCamera->getLookAt() + moveAmount);
    mCamera->setPosition(mCamera->getPosition() + moveAmount);

    mIsComputeLengthCameraToLookAt = true;
}

void EngineCamera::rotateLookAtPoint(const IMouse& mouse, const Vector2& mouseMoveAmount) {
    if (!canRotate(mouse, mouseMoveAmount)) {
        return;
    }

    //新しい回転軸を計算する
    computeRotation(mouseMoveAmount);

    //カメラから注視点までの距離を計算する
    computeLengthCameraToLookAt();

    mCamera->setPosition(mCamera->getLookAt() + -forward() * mLengthCameraToLookAt);
}

void EngineCamera::computeRotation(const Vector2& mouseMoveAmount) {
    //マウス移動量から回転軸を求める
    auto rotAxis = Vector3::normalize(Vector3(mouseMoveAmount.y, mouseMoveAmount.x, 0.f));
    rotAxis = Vector3::transform(rotAxis, mCameraRotation);
    //回転軸とマウス移動量からクォータニオンを求める
    Quaternion r(rotAxis, mouseMoveAmount.length() * ROTATE_SPEED);

    mCameraRotation = Quaternion::concatenate(mCameraRotation, r);
}

void EngineCamera::zoomCamera(const IMouse& mouse) {
    int scroll = mouse.getMouseScrollWheel();
    if (scroll > 0) {
        zoomIn(mouse);
    } else if (scroll < 0) {
        zoomOut(mouse);
    }
}

void EngineCamera::zoomIn(const IMouse& mouse) {
    mCamera->setPosition(mCamera->getPosition() + forward() * ZOOM_SPEED);
    mIsComputeLengthCameraToLookAt = true;
}

void EngineCamera::zoomOut(const IMouse& mouse) {
    mCamera->setPosition(mCamera->getPosition() + -forward() * ZOOM_SPEED);
    mIsComputeLengthCameraToLookAt = true;
}

Vector3 EngineCamera::getCameraToLookAt() const {
    return (mCamera->getLookAt() - mCamera->getPosition());
}

void EngineCamera::computeLengthCameraToLookAt() {
    //計算フラグが立っていたら計算する
    if (mIsComputeLengthCameraToLookAt) {
        mLengthCameraToLookAt = getCameraToLookAt().length();
        mIsComputeLengthCameraToLookAt = false;
    }
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
