#include "PlayerMove.h"
#include "PlayerAnimationController.h"
#include "../../Engine/Camera/Camera.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Time.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectManager.h"
#include "../../../Input/Input.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"

PlayerMove::PlayerMove(GameObject& gameObject)
    : Component(gameObject)
    , mCamera(nullptr)
    , mAnimation(nullptr)
    , mWalkSpeed(0.f)
    , mIsMoving(false)
{
}

PlayerMove::~PlayerMove() = default;

void PlayerMove::start() {
    const auto& cam = gameObject().getGameObjectManager().find("Camera");
    mCamera = cam->componentManager().getComponent<Camera>();
    mAnimation = getComponent<SkinMeshComponent>();
}

void PlayerMove::update() {
    auto p1 = transform().getPosition() + Vector3::up * 10.f;
    Debug::renderLine(p1, p1 + transform().forward() * 10.f, ColorPalette::blue);

    const auto& leftStick = Input::joyPad().leftStick();
    if (isMovable(leftStick)) {
        move(leftStick);
    } else {
        stop();
    }
}

void PlayerMove::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "walkSpeed", &mWalkSpeed);
}

void PlayerMove::move(const Vector2& leftStickValue) {
    //カメラの視線ベクトルから向きを得る
    auto eye = Vector3::normalize(mCamera->getLookAt() - mCamera->getPosition());
    auto lookRot = Quaternion::lookRotation(eye);
    //カメラ向きから右、前方ベクトルを求める
    auto camRight = Vector3::transform(Vector3::right, lookRot);
    camRight.y = 0.f;
    auto camForward = Vector3::transform(Vector3::forward, lookRot);
    camForward.y = 0.f;

    auto& t = transform();
    auto moveDir = camRight * leftStickValue.x + camForward * leftStickValue.y;
    t.translate(moveDir * mWalkSpeed * Time::deltaTime);
    t.setRotation(Quaternion::lookRotation(moveDir));

    if (!mIsMoving) {
        mAnimation->changeMotion(PlayerMotions::WALK);
        mAnimation->setLoop(true);
        mIsMoving = true;
    }
}

void PlayerMove::stop() {
    if (mIsMoving) {
        mAnimation->changeMotion(PlayerMotions::IDOL);
        mAnimation->setLoop(true);
        mIsMoving = false;
    }
}

bool PlayerMove::isMovable(const Vector2& leftStickValue) const {
    return !(Vector2::equal(leftStickValue, Vector2::zero));
}
