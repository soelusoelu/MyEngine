#include "PlayerRoll.h"
#include "PlayerAnimationController.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Time.h"
#include "../../../Input/Input.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"

PlayerRoll::PlayerRoll(GameObject& gameObject)
    : Component(gameObject)
    , mAnimation(nullptr)
    , mRollingDistance(0.f)
    , mIsRolling(false)
    , mRollingMotionTime(std::make_unique<Time>())
    , mRollingStartPoint()
    , mRollingEndPoint()
{
}

PlayerRoll::~PlayerRoll() = default;

void PlayerRoll::start() {
    mAnimation = getComponent<SkinMeshComponent>();

    const auto& rollingMotion = mAnimation->getMotion(PlayerMotions::ROLL);
    auto limit = static_cast<float>(rollingMotion.numFrame) / 60.f;
    mRollingMotionTime->setLimitTime(limit);
}

void PlayerRoll::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "rollingDistance", &mRollingDistance);
}

void PlayerRoll::originalUpdate() {
    auto& t = transform();

    if (Input::joyPad().getJoyUp(JoyCode::B)) {
        mAnimation->changeMotion(PlayerMotions::ROLL);
        mAnimation->setLoop(false);
        mIsRolling = true;
        mRollingStartPoint = t.getPosition();
        mRollingEndPoint = t.getPosition() + t.forward() * mRollingDistance;
    }

    if (mIsRolling) {
        mRollingMotionTime->update();
        if (mRollingMotionTime->isTime()) {
            mRollingMotionTime->reset();
            mAnimation->changeMotion(PlayerMotions::IDOL);
            mIsRolling = false;
            return;
        }

        t.setPosition(Vector3::lerp(mRollingStartPoint, mRollingEndPoint, mRollingMotionTime->rate()));
    }
}

bool PlayerRoll::isRolling() const {
    return mIsRolling;
}
