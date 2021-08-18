#include "OctopusFoot.h"
#include "OctopusFootCommonSetting.h"
#include "../../Engine/Collider/OBBCollider.h"
#include "../../Engine/Mesh/MeshComponent.h"
#include "../../../Math/Math.h"
#include "../../../Utility/JsonHelper.h"
#include <algorithm>

OctopusFoot::OctopusFoot()
    : Component()
    , mMesh(nullptr)
    , mFootMeshNumber(-1)
    , mHp(0)
{
}

OctopusFoot::~OctopusFoot() = default;

void OctopusFoot::start() {
    mMesh = getComponent<MeshComponent>();
    auto commonSettings = getComponent<OctopusFootCommonSetting>();
    mHp = commonSettings->mHp;

    //全コライダーから対象ボーンのコライダーを抜き出す
    auto colliders = getComponents<OBBCollider>();
    for (const auto& c : colliders) {
        const auto no = c->getBone().number;
        const auto result = std::find(mTargetBoneNo.begin(), mTargetBoneNo.end(), no);
        if (result != mTargetBoneNo.end()) {
            mColliders.emplace_back(c);
        }
    }
}

void OctopusFoot::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mTargetBoneNo, "targetBoneNo", inObj, alloc, mode);
    JsonHelper::getSet(mFootMeshNumber, "footMeshNumber", inObj, alloc, mode);
}

const OBBColliderPtrArray& OctopusFoot::getColliders() const {
    return mColliders;
}

void OctopusFoot::takeDamage(int damage) {
    mHp -= damage;

    //足の色を赤くしていく
    changeColor();

    if (isDestroy()) {
        //足死亡
        destroyFoot();
        mOnDestroyFoot(*this);
    }
}

int OctopusFoot::getHp() const {
    return mHp;
}

bool OctopusFoot::isDestroy() const {
    return (mHp <= 0);
}

int OctopusFoot::getNumber() const {
    return mFootMeshNumber;
}

void OctopusFoot::onDestroyFoot(const std::function<void(const OctopusFoot&)>& f) {
    mOnDestroyFoot += f;
}

void OctopusFoot::destroyFoot() {
    //足に付属するコライダーを全て無効化する
    for (const auto& c : mColliders) {
        c->disabled();
    }

    //足を非アクティブにする
    mMesh->getMesh()->setMeshActive(mFootMeshNumber, false);
}

void OctopusFoot::changeColor() {
    if (isDestroy()) {
        return;
    }

    auto mesh = mMesh->getMesh();
    auto mat = mesh->getMaterial(mFootMeshNumber);
    mat.diffuse = ColorPalette::red * Math::lerp(0.f, 1.f, 1.f / static_cast<float>(mHp));
    mesh->setMaterial(mat, mFootMeshNumber);
}
