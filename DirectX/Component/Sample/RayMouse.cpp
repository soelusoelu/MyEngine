#include "RayMouse.h"
#include "../Camera/Camera.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Input/Input.h"

RayMouse::RayMouse(GameObject& gameObject) :
    Component(gameObject),
    mCamera(nullptr),
    mClickPos(Vector3::zero)
{
}

RayMouse::~RayMouse() = default;

void RayMouse::start() {
    mCamera = gameObject().getGameObjectManager().find("Camera")->componentManager().getComponent<Camera>();
}

void RayMouse::update() {
    const auto& mouse = Input::mouse();
    if (mouse.getMouseButton(MouseCode::LeftButton)) {
        mClickPos = mCamera->screenToWorldPoint(mouse.getMousePosition(), 1.f);
        Debug::renderLine(Vector3(0.f, 100.f, 0.f), mClickPos, ColorPalette::red);
    }
}
