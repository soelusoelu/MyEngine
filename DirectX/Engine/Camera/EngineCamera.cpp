#include "EngineCamera.h"
#include "SimpleCamera.h"
#include "../../Input/Input.h"

EngineCamera::EngineCamera()
    : mCamera(std::make_unique<SimpleCamera>())
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
    if (!mouse.getMouseButton(MouseCode::RightButton)) {
        return;
    }

    //1フレームのマウス移動量を取得
    const auto& amount = mouse.getMouseMoveAmount();

    //マウスが移動していないなら終了
    if (Vector2::equal(amount, Vector2::zero)) {
        return;
    }

    auto cameraToLookAt = mCamera->getLookAt() - mCamera->getPosition();
    auto normalizeCameraToLookAt = Vector3::normalize(cameraToLookAt);
    auto lengthCameraToLookAt = cameraToLookAt.length();

    mCamera->setPosition(Vector3(mCamera->getLookAt() - normalizeCameraToLookAt * lengthCameraToLookAt));
}
