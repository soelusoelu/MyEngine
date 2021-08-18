#include "HitPoint.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Utility/JsonHelper.h"

HitPoint::HitPoint()
    : Component()
    , mHp(0)
    , mMaxHp(0)
{
}

HitPoint::~HitPoint() = default;

void HitPoint::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mHp, "HP", inObj, alloc, mode);
    JsonHelper::getSet(mMaxHp, "maxHP", inObj, alloc, mode);

    if (mode == FileMode::LOAD) {
        if (mMaxHp < mHp) {
            mMaxHp = mHp;
        }
    }
}

void HitPoint::drawInspector() {
    if (ImGuiWrapper::sliderInt("HP", mHp, 0, mMaxHp)) {
        mCallbackUpdateHp(*this);
    }
    if (ImGuiWrapper::dragInt("MaxHP", mMaxHp, 1.f, mHp)) {
        mCallbackUpdateHp(*this);
    }
}

void HitPoint::takeDamage(int damage) {
    if (damage < 0) {
        damage = 0;
    }
    mHp -= damage;
    if (mHp < 0) {
        mHp = 0;
    }

    mCallbackUpdateHp(*this);
}

void HitPoint::takeHeal(int heal) {
    if (heal < 0) {
        heal = 0;
    }
    mHp += heal;
    clampHpIfOverMax();

    mCallbackUpdateHp(*this);
}

void HitPoint::setHp(int hp, bool isChangeMax) {
    mHp = hp;

    //最大値も変更するか
    if (isChangeMax) {
        mMaxHp = hp;
    }

    clampHpIfOverMax();

    mCallbackUpdateHp(*this);
}

int HitPoint::getHp() const {
    return mHp;
}

int HitPoint::getMaxHp() const {
    return mMaxHp;
}

float HitPoint::getHpRate() const {
    return static_cast<float>(mHp) / static_cast<float>(mMaxHp);
}

void HitPoint::callbackChangeHp(const std::function<void(const HitPoint&)>& callback) {
    mCallbackUpdateHp += callback;
}

void HitPoint::clampHpIfOverMax() {
    if (mHp > mMaxHp) {
        mHp = mMaxHp;
    }
}
