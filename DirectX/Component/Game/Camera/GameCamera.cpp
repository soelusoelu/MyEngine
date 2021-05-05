#include "GameCamera.h"
#include "../../Engine/Camera/Camera.h"
#include "../../../Device/Time.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectManager.h"
#include "../../../Input/Input.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"

GameCamera::GameCamera()
    : Component()
    , mCamera(nullptr)
    , mPlayer(nullptr)
    , mRotateSpeed(0.f)
    , mToPlayerDistance(0.f)
    , mLookAtOffsetY(0.f)
{
}

GameCamera::~GameCamera() = default;

void GameCamera::start() {
    const auto& cam = gameObject().getGameObjectManager().find("Camera");
    mCamera = cam->componentManager().getComponent<Camera>();
}

void GameCamera::update() {
    if (!mPlayer) {
        Debug::logError("Player is null.");
        return;
    }

    const auto& pt = mPlayer->transform();
    auto lookAt = pt.getPosition() + Vector3::up * mLookAtOffsetY;
    mCamera->lookAt(lookAt);


    const auto& rightStick = Input::joyPad().rightStick();
    auto& t = transform();
    if (!Vector2::equal(rightStick, Vector2::zero)) {
        t.rotate(Vector3(rightStick.y, -rightStick.x, 0.f) * mRotateSpeed * Time::deltaTime);
    }

    mCamera->setPosition(lookAt + -t.forward() * mToPlayerDistance);
}

void GameCamera::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "rotateSpeed", &mRotateSpeed);
    JsonHelper::getFloat(inObj, "cameraToPlayerDistance", &mToPlayerDistance);
    JsonHelper::getFloat(inObj, "lookAtOffsetY", &mLookAtOffsetY);
}

void GameCamera::setPlayer(const std::shared_ptr<GameObject>& player) {
    mPlayer = player;

    const auto& pt = mPlayer->transform();
    const auto& pp = pt.getPosition();

    auto lookAt = pp + Vector3::up * mLookAtOffsetY;
    auto pos = pp + -pt.forward() * mToPlayerDistance + Vector3::up * 15.f;

    auto eye = Vector3::normalize(lookAt - pos);
    transform().setRotation(Quaternion::lookRotation(eye));
}
