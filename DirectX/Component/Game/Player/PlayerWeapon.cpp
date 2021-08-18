#include "PlayerWeapon.h"
#include "../../Engine/Collider/AABBCollider.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Transform/ParentChildRelationship.h"
#include "../../../Transform/Transform3D.h"

PlayerWeapon::PlayerWeapon()
    : Component()
    , mWeapon(nullptr)
    , mWeaponCollider(nullptr)
{
}

PlayerWeapon::~PlayerWeapon() = default;

void PlayerWeapon::setWeapon(const std::shared_ptr<GameObject>& weapon) {
    mWeapon = weapon;

    //武器の当たり判定を無効化する
    mWeaponCollider = weapon->componentManager().getComponent<AABBCollider>();
    mWeaponCollider->disabled();

    //装備を設定する
    const auto& rightHand = getComponent<SkinMeshComponent>()->getBoneCurrentFrameMatrix()[RIGHT_HAND_BONE_NO];
    transform().getParentChildRelation().setEquipment(weapon, &rightHand);
}

const GameObject& PlayerWeapon::getWeapon() const {
    return *mWeapon;
}

AABBCollider& PlayerWeapon::getWeaponCollider() const {
    return *mWeaponCollider;
}
