#include "PlayerAttack.h"
#include "PlayerMotions.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Time.h"
#include "../../../Input/Input.h"
#include "../../../Utility/LevelLoader.h"

PlayerAttack::PlayerAttack()
    : Component()
    , mAnimation(nullptr)
    , mAttackMotionElapsedTimer(std::make_unique<Time>())
    , mAttackMotionTime{ 0.f, 0.f, 0.f, 0.f }
    , mLowestCoolTimeUpToAdditionalAttack{ 0.f, 0.f }
    , mIsFirstAttackMiddle(false)
    , mIsSecondAttackMiddle(false)
    , mIsEndAttackMiddle(false)
{
}

PlayerAttack::~PlayerAttack() = default;

void PlayerAttack::start() {
    mAnimation = getComponent<SkinMeshComponent>();

    const auto& firstAttack = mAnimation->getMotion(PlayerMotions::FIRST_ATTACK_START);
    mAttackMotionTime[FIRST_ATTACK_START_NO] = static_cast<float>(firstAttack.numFrame) / 60.f;
    const auto& secondAttack = mAnimation->getMotion(PlayerMotions::SECOND_ATTACK_START);
    mAttackMotionTime[SECOND_ATTACK_START_NO] = static_cast<float>(secondAttack.numFrame) / 60.f;
    const auto& firstAttackEnd = mAnimation->getMotion(PlayerMotions::FIRST_ATTACK_END);
    mAttackMotionTime[FIRST_ATTACK_END_NO] = static_cast<float>(firstAttackEnd.numFrame) / 60.f;
    const auto& secondAttackEnd = mAnimation->getMotion(PlayerMotions::SECOND_ATTACK_END);
    mAttackMotionTime[SECOND_ATTACK_END_NO] = static_cast<float>(secondAttackEnd.numFrame) / 60.f;
}

void PlayerAttack::update() {
    if (mIsFirstAttackMiddle || mIsSecondAttackMiddle) {
        attackEnd();
    }

    if (mIsEndAttackMiddle) {
        updateEndAttack();
    }
}

void PlayerAttack::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(
        inObj,
        "firstLowestCoolTimeUpToAdditionalAttack",
        &mLowestCoolTimeUpToAdditionalAttack[FIRST_ATTACK_START_NO]
    );
    JsonHelper::getFloat(
        inObj,
        "secondLowestCoolTimeUpToAdditionalAttack",
        &mLowestCoolTimeUpToAdditionalAttack[SECOND_ATTACK_START_NO]
    );
}

void PlayerAttack::originalUpdate() {
    if (!Input::joyPad().getJoyDown(JoyCode::RightButton)) {
        return;
    }

    if (canFirstAttack()) {
        firstAttack();
    }
    if (canSecondAttack()) {
        secondAttack();
    }
}

bool PlayerAttack::isAttacking() const {
    if (mIsFirstAttackMiddle) {
        return true;
    }
    if (mIsSecondAttackMiddle) {
        return true;
    }
    if (mIsEndAttackMiddle) {
        return true;
    }

    return false;
}

void PlayerAttack::updateEndAttack() {
    if (!updateTimer()) {
        return;
    }

    mAnimation->changeMotion(PlayerMotions::IDOL);
    mAnimation->setLoop(true);

    mIsEndAttackMiddle = false;
}

void PlayerAttack::firstAttack() {
    mAnimation->changeMotion(PlayerMotions::FIRST_ATTACK_START);
    mAnimation->setLoop(false);
    mIsFirstAttackMiddle = true;
    mIsSecondAttackMiddle = false;
    mAttackMotionElapsedTimer->setLimitTime(mAttackMotionTime[FIRST_ATTACK_START_NO]);
    mAttackMotionElapsedTimer->reset();
}

void PlayerAttack::secondAttack() {
    mAnimation->changeMotion(PlayerMotions::SECOND_ATTACK_START);
    mAnimation->setLoop(false);
    mIsFirstAttackMiddle = false;
    mIsSecondAttackMiddle = true;
    mAttackMotionElapsedTimer->setLimitTime(mAttackMotionTime[SECOND_ATTACK_START_NO]);
    mAttackMotionElapsedTimer->reset();
}

void PlayerAttack::attackEnd() {
    if (!updateTimer()) {
        return;
    }

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
    mAttackMotionElapsedTimer->setLimitTime(mAttackMotionTime[FIRST_ATTACK_END_NO]);
    mAttackMotionElapsedTimer->reset();
}

void PlayerAttack::secondAttackEnd() {
    mAnimation->changeMotion(PlayerMotions::SECOND_ATTACK_END);
    mAnimation->setLoop(false);
    mIsEndAttackMiddle = true;
    mAttackMotionElapsedTimer->setLimitTime(mAttackMotionTime[SECOND_ATTACK_END_NO]);
    mAttackMotionElapsedTimer->reset();
}

bool PlayerAttack::updateTimer() {
    mAttackMotionElapsedTimer->update();

    if (mAttackMotionElapsedTimer->isTime()) {
        mAttackMotionElapsedTimer->reset();
        return true;
    }

    return false;
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
    if (mAttackMotionElapsedTimer->getCountUpTime() < mLowestCoolTimeUpToAdditionalAttack[FIRST_ATTACK_START_NO]) {
        return false;
    }
    if (mAttackMotionElapsedTimer->isTime()) {
        mAttackMotionElapsedTimer->reset();
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
