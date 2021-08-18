#pragma once

#include "../../Component.h"

class WeaponDamage
    : public Component
{
public:
    WeaponDamage();
    ~WeaponDamage();
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    int getDamage() const;

private:
    WeaponDamage(const WeaponDamage&) = delete;
    WeaponDamage& operator=(const WeaponDamage&) = delete;

private:
    int mDamage;
};
