#pragma once

#include "../../Component.h"
#include <memory>

class GameObject;
class AABBCollider;

class PlayerWeapon
    : public Component
{
public:
    PlayerWeapon();
    ~PlayerWeapon();
    void setWeapon(const std::shared_ptr<GameObject>& weapon);
    const GameObject& getWeapon() const;
    AABBCollider& getWeaponCollider() const;

private:
    PlayerWeapon(const PlayerWeapon&) = delete;
    PlayerWeapon& operator=(const PlayerWeapon&) = delete;

private:
    std::shared_ptr<GameObject> mWeapon;
    std::shared_ptr<AABBCollider> mWeaponCollider;

    static constexpr unsigned RIGHT_HAND_BONE_NO = 9;
};
