#include "PlayerAttack.h"
#include "PlayerAnimationController.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Time.h"
#include "../../../Input/Input.h"
#include "../../../Utility/LevelLoader.h"

PlayerAttack::PlayerAttack(GameObject& gameObject)
    : Component(gameObject)
    , mAnimation(nullptr)
    , mAttackMotionElapsedTime(std::make_unique<Time>())
    , mAdditionalAttackTimer{ 0.f, 0.f, 0.f, 0.f }
    , mIsFirstAttackMiddle(false)
    , mIsSecondAttackMiddle(false)
{
}

PlayerAttack::~PlayerAttack() = default;

void PlayerAttack::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    const auto& firstAttack = mAnimation->getMotion(PlayerMotionName::FIRST_ATTACK);
    mAdditionalAttackTimer[0].receptionTimeUpToAdditionalAttack = static_cast<float>(firstAttack.numFrame) / 60.f;
    mAttackMotionElapsedTime->setLimitTime(mAdditionalAttackTimer[0].receptionTimeUpToAdditionalAttack);
    const auto& secondAttack = mAnimation->getMotion(PlayerMotionName::SECOND_ATTACK);
    mAdditionalAttackTimer[1].receptionTimeUpToAdditionalAttack = static_cast<float>(secondAttack.numFrame) / 60.f;
}

void PlayerAttack::update() {
    if (Input::joyPad().getJoyDown(JoyCode::RightButton)) {
        updateAttack();
    }

    if (mIsFirstAttackMiddle || mIsSecondAttackMiddle) {
        attackEnd();
    }
}

void PlayerAttack::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "firstLowestCoolTimeUpToAdditionalAttack", &mAdditionalAttackTimer[0].lowestCoolTimeUpToAdditionalAttack);
    JsonHelper::getFloat(inObj, "secondLowestCoolTimeUpToAdditionalAttack", &mAdditionalAttackTimer[1].lowestCoolTimeUpToAdditionalAttack);
}

void PlayerAttack::updateAttack() {
    if (isFirstAttackable()) {
        firstAttack();
    }
    if (isSecondAttackable()) {
        secondAttack();
    }
}

void PlayerAttack::firstAttack() {
    mAnimation->changeMotion(PlayerMotionName::FIRST_ATTACK);
    mAnimation->setLoop(false);
    mIsFirstAttackMiddle = true;
    mIsSecondAttackMiddle = false;
    mAttackMotionElapsedTime->setLimitTime(mAdditionalAttackTimer[0].receptionTimeUpToAdditionalAttack);
    mAttackMotionElapsedTime->reset();
}

void PlayerAttack::secondAttack() {
    mAnimation->changeMotion(PlayerMotionName::SECOND_ATTACK);
    mAnimation->setLoop(false);
    mIsFirstAttackMiddle = false;
    mIsSecondAttackMiddle = true;
    mAttackMotionElapsedTime->setLimitTime(mAdditionalAttackTimer[1].receptionTimeUpToAdditionalAttack);
    mAttackMotionElapsedTime->reset();
}

void PlayerAttack::attackEnd() {
    mAttackMotionElapsedTime->update();
    if (mAttackMotionElapsedTime->isTime()) {
        mAttackMotionElapsedTime->reset();
        mIsFirstAttackMiddle = false;
        mIsSecondAttackMiddle = false;
        mAnimation->changeMotion(PlayerMotionName::IDOL);
    }
}

bool PlayerAttack::isFirstAttackable() const {
    if (mIsFirstAttackMiddle) {
        return false;
    }
    if (mIsSecondAttackMiddle) {
        return false;
    }

    return true;
}

bool PlayerAttack::isSecondAttackable() const {
    if (!mIsFirstAttackMiddle) {
        return false;
    }
    if (mIsSecondAttackMiddle) {
        return false;
    }
    if (mAttackMotionElapsedTime->getCountUpTime() < mAdditionalAttackTimer[0].lowestCoolTimeUpToAdditionalAttack) {
        return false;
    }
    if (mAttackMotionElapsedTime->isTime()) {
        mAttackMotionElapsedTime->reset();
        return false;
    }

    return true;
}
