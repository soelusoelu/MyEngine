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
    , mAdditionalAttackTimer{ 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f }
    , mIsFirstAttackMiddle(false)
    , mIsSecondAttackMiddle(false)
    , mIsEndAttackMiddle(false)
{
}

PlayerAttack::~PlayerAttack() = default;

void PlayerAttack::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    const auto& firstAttack = mAnimation->getMotion(PlayerMotions::FIRST_ATTACK_START);
    mAdditionalAttackTimer[0].motionTime = static_cast<float>(firstAttack.numFrame) / 60.f;
    mAttackMotionElapsedTime->setLimitTime(mAdditionalAttackTimer[0].motionTime);
    const auto& secondAttack = mAnimation->getMotion(PlayerMotions::SECOND_ATTACK_START);
    mAdditionalAttackTimer[1].motionTime = static_cast<float>(secondAttack.numFrame) / 60.f;

    const auto& firstAttackEnd = mAnimation->getMotion(PlayerMotions::FIRST_ATTACK_END);
    mAdditionalAttackTimer[2].motionTime = static_cast<float>(firstAttackEnd.numFrame) / 60.f;
    const auto& secondAttackEnd = mAnimation->getMotion(PlayerMotions::SECOND_ATTACK_END);
    mAdditionalAttackTimer[3].motionTime = static_cast<float>(firstAttackEnd.numFrame) / 60.f;
}

void PlayerAttack::update() {
    if (Input::joyPad().getJoyDown(JoyCode::RightButton)) {
        updateAttack();
    }

    if (mIsFirstAttackMiddle || mIsSecondAttackMiddle) {
        attackEnd();
    }

    if (mIsEndAttackMiddle) {
        updateEndAttack();
    }
}

void PlayerAttack::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "firstLowestCoolTimeUpToAdditionalAttack", &mAdditionalAttackTimer[0].lowestCoolTimeUpToAdditionalAttack);
    JsonHelper::getFloat(inObj, "secondLowestCoolTimeUpToAdditionalAttack", &mAdditionalAttackTimer[1].lowestCoolTimeUpToAdditionalAttack);
}

void PlayerAttack::updateAttack() {
    if (canFirstAttack()) {
        firstAttack();
    }
    if (canSecondAttack()) {
        secondAttack();
    }
}

void PlayerAttack::updateEndAttack() {
    mAttackMotionElapsedTime->update();

    //モーション中は無視
    if (!mAttackMotionElapsedTime->isTime()) {
        return;
    }

    //タイマーリセット
    mAttackMotionElapsedTime->reset();

    mAnimation->changeMotion(PlayerMotions::IDOL);

    mIsEndAttackMiddle = false;
}

void PlayerAttack::firstAttack() {
    mAnimation->changeMotion(PlayerMotions::FIRST_ATTACK_START);
    mAnimation->setLoop(false);
    mIsFirstAttackMiddle = true;
    mIsSecondAttackMiddle = false;
    mAttackMotionElapsedTime->setLimitTime(mAdditionalAttackTimer[0].motionTime);
    mAttackMotionElapsedTime->reset();
}

void PlayerAttack::secondAttack() {
    mAnimation->changeMotion(PlayerMotions::SECOND_ATTACK_START);
    mAnimation->setLoop(false);
    mIsFirstAttackMiddle = false;
    mIsSecondAttackMiddle = true;
    mAttackMotionElapsedTime->setLimitTime(mAdditionalAttackTimer[1].motionTime);
    mAttackMotionElapsedTime->reset();
}

void PlayerAttack::attackEnd() {
    mAttackMotionElapsedTime->update();

    if (!mAttackMotionElapsedTime->isTime()) {
        return;
    }

    mAttackMotionElapsedTime->reset();

    //1回目の攻撃終了か
    if (isEndFirstAttack()) {
        firstAttackEnd();
    }
    //2回目の攻撃終了か
    if (isEndSecondAttack()) {
        secondAttackEnd();
    }

    //攻撃状態リセット
    mIsFirstAttackMiddle = false;
    mIsSecondAttackMiddle = false;
}

void PlayerAttack::firstAttackEnd() {
    mAnimation->changeMotion(PlayerMotions::FIRST_ATTACK_END);
    mAnimation->setLoop(false);
    mIsEndAttackMiddle = true;
    mAttackMotionElapsedTime->setLimitTime(mAdditionalAttackTimer[2].motionTime);
    mAttackMotionElapsedTime->reset();
}

void PlayerAttack::secondAttackEnd() {
    mAnimation->changeMotion(PlayerMotions::SECOND_ATTACK_END);
    mAnimation->setLoop(false);
    mIsEndAttackMiddle = true;
    mAttackMotionElapsedTime->setLimitTime(mAdditionalAttackTimer[3].motionTime);
    mAttackMotionElapsedTime->reset();
}

bool PlayerAttack::canFirstAttack() const {
    if (mIsFirstAttackMiddle) {
        return false;
    }
    if (mIsSecondAttackMiddle) {
        return false;
    }

    return true;
}

bool PlayerAttack::canSecondAttack() const {
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

bool PlayerAttack::isEndFirstAttack() const {
    if (!mIsFirstAttackMiddle) {
        return false;
    }
    if (mIsSecondAttackMiddle) {
        return false;
    }

    return true;
}

bool PlayerAttack::isEndSecondAttack() const {
    if (mIsFirstAttackMiddle) {
        return false;
    }
    if (!mIsSecondAttackMiddle) {
        return false;
    }

    return true;
}
