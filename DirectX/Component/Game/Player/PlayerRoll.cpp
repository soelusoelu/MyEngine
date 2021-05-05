#include "PlayerRoll.h"
#include "PlayerMotions.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Time.h"
#include "../../../Input/Input.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"

PlayerRoll::PlayerRoll()
    : Component()
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

void PlayerRoll::update() {
    if (!mIsRolling) {
        return;
    }

    mRollingMotionTime->update();
    if (mRollingMotionTime->isTime()) {
        mRollingMotionTime->reset();
        mAnimation->changeMotion(PlayerMotions::IDOL);
        mIsRolling = false;
        return;
    }

    transform().setPosition(Vector3::lerp(mRollingStartPoint, mRollingEndPoint, mRollingMotionTime->rate()));
}

void PlayerRoll::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "rollingDistance", &mRollingDistance);
}

void PlayerRoll::originalUpdate() {
    if (Input::joyPad().getJoyUp(JoyCode::B)) {
        mAnimation->changeMotion(PlayerMotions::ROLL);
        mAnimation->setLoop(false);
        mIsRolling = true;

        auto& t = transform();
        mRollingStartPoint = t.getPosition();
        mRollingEndPoint = t.getPosition() + t.forward() * mRollingDistance;
    }
}

bool PlayerRoll::isRolling() const {
    return mIsRolling;
}
