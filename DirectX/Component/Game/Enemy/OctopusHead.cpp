#include "OctopusHead.h"
#include "../../Engine/Collider/OBBCollider.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Utility/JsonHelper.h"

OctopusHead::OctopusHead()
    : Component()
    , mIsDestroy(false)
{
}

OctopusHead::~OctopusHead() = default;

void OctopusHead::start() {
    //全コライダーから対象ボーンのコライダーを抜き出す
    auto colliders = getComponents<OBBCollider>();
    for (const auto& c : colliders) {
        auto no = c->getBone().number;
        auto result = std::find(mTargetBoneNo.begin(), mTargetBoneNo.end(), no);
        if (result != mTargetBoneNo.end()) {
            mColliders.emplace_back(c);
        }
    }
}

void OctopusHead::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mTargetBoneNo, "targetBoneNo", inObj, alloc, mode);
}

const OBBColliderPtrArray& OctopusHead::getColliders() const {
    return mColliders;
}

void OctopusHead::takeDamage(int damage) {
    mIsDestroy = true;

    Debug::log("death");
}

int OctopusHead::getHp() const {
    //頭はHP1で固定だから生きてたら1、死んでたら0
    return (isDestroy()) ? 0 : 1;
}

bool OctopusHead::isDestroy() const {
    return mIsDestroy;
}
