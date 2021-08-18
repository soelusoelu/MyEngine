#include "Stamina.h"
#include "../../../Device/Time.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Math/Math.h"
#include "../../../Utility/JsonHelper.h"

Stamina::Stamina()
    : Component()
    , mCurrentStamina(0.f)
    , mMaxStamina(0.f)
    , mHealAmount(0.f)
    , mUsingStaminaThisFrame(false)
    , mHealFlag(true)
    , mHealRate(DEFAULT_HEAL_RATE)
    , mCoolTime(std::make_unique<Time>())
{
}

Stamina::~Stamina() = default;

void Stamina::lateUpdate() {
    if (canHeal()) {
        mCurrentStamina += mHealAmount * mHealRate * Time::deltaTime;
        mCurrentStamina = Math::Min(mCurrentStamina, mMaxStamina);

        mCallbackChangeStamina(*this);
    }

    if (mUsingStaminaThisFrame) {
        mUsingStaminaThisFrame = false;
    }
}

void Stamina::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mCurrentStamina, "stamina", inObj, alloc, mode);
    mMaxStamina = mCurrentStamina;
    JsonHelper::getSet(mHealAmount, "healAmount", inObj, alloc, mode);
    mCoolTime->saveAndLoad(inObj, alloc, mode);
}

void Stamina::drawInspector() {
    if (ImGuiWrapper::sliderFloat("stamina", mCurrentStamina, 0.f, mMaxStamina)) {
        mCallbackChangeStamina(*this);
    }
}

bool Stamina::use(float amount) {
    if (!canUse()) {
        return false;
    }

    mCurrentStamina -= amount;
    if (mCurrentStamina <= 0.f) {
        mCurrentStamina = 0.f;

        mCoolTime->reset();
    }

    mUsingStaminaThisFrame = true;

    mCallbackChangeStamina(*this);

    return true;
}

void Stamina::setHealRate(float rate) {
    mHealRate = rate;
}

void Stamina::setHealRateToDefault() {
    mHealRate = DEFAULT_HEAL_RATE;
}

void Stamina::setHealFlag(bool value) {
    mHealFlag = value;
}

float Stamina::getStamina() const {
    return mCurrentStamina;
}

float Stamina::getStaminaRate() const {
    return (mCurrentStamina / mMaxStamina);
}

void Stamina::callbackChangeStamina(const std::function<void(const Stamina&)>& callback) {
    mCallbackChangeStamina += callback;
}

bool Stamina::canUse() const {
    return (mCurrentStamina > 0.f);
}

bool Stamina::canHeal() const {
    if (!mHealFlag) {
        return false;
    }
    if (!mCoolTime->isTime()) {
        mCoolTime->update();
        return false;
    }
    if (mUsingStaminaThisFrame) {
        return false;
    }
    if (Math::equal(mCurrentStamina, mMaxStamina)) {
        return false;
    }

    return true;
}
