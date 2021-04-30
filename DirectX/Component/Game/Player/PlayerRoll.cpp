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
    , mIsDuringRolling(false)
    , mRollingMotionTime(std::make_unique<Time>())
{
}

PlayerRoll::~PlayerRoll() = default;

void PlayerRoll::start() {
    mAnimation = getComponent<SkinMeshComponent>();

    const auto& rollingMotion = mAnimation->getMotion(PlayerMotionName::ROLL);
    auto limit = static_cast<float>(rollingMotion.numFrame) / 60.f;
    mRollingMotionTime->setLimitTime(limit);
}

void PlayerRoll::update() {
    if (Input::joyPad().getJoyDown(JoyCode::B)) {
        mAnimation->changeMotion(PlayerMotionName::ROLL);
        mAnimation->setLoop(false);
        mIsDuringRolling = true;
    }

    if (mIsDuringRolling) {
        mRollingMotionTime->update();
        if (mRollingMotionTime->isTime()) {
            mRollingMotionTime->reset();
            mAnimation->changeMotion(PlayerMotionName::IDOL);
            mIsDuringRolling = false;
            return;
        }

        auto& t = transform();
        t.translate(t.forward() * mRollingMotionTime->rate() * mRollingDistance * Time::deltaTime);
    }
}

void PlayerRoll::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "rollingDistance", &mRollingDistance);
}
