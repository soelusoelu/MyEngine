#include "WeaponDamage.h"
#include "../../../Utility/JsonHelper.h"

WeaponDamage::WeaponDamage()
    : Component()
    , mDamage(0)
{
}

WeaponDamage::~WeaponDamage() = default;

void WeaponDamage::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mDamage, "damage", inObj, alloc, mode);
}

int WeaponDamage::getDamage() const {
    return mDamage;
}
