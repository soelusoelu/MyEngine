#include "AABBAnimationCollider.h"
#include "../Mesh/AnimationCPU.h"
#include "../Mesh/MeshComponent.h"
#include "../../../Device/Physics.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include <cassert>

AABBAnimationCollider::AABBAnimationCollider(GameObject& gameObject)
    : Collider(gameObject)
    , mMesh(nullptr)
    , mAnimationCPU(nullptr)
    , mIsRenderCollision(true)
{
}

AABBAnimationCollider::~AABBAnimationCollider() = default;

void AABBAnimationCollider::start() {
    Collider::start();

    mMesh = getComponent<MeshComponent>();
    mAnimationCPU = getComponent<AnimationCPU>();
    if (!mAnimationCPU) {
        mAnimationCPU = addComponent<AnimationCPU>("AnimationCPU");
    }

    //基礎となるAABBを作成する
    createAABB();
    //最新のAABBの点を計算する
    updatePoints();

    if (mPhysics) {
        //mPhysics->add(shared_from_this());
    }
}

void AABBAnimationCollider::lateUpdate() {
    Collider::lateUpdate();

    //AABBを更新する
    computeAABB();

    //AABBの点を更新する
    updatePoints();

    //当たり判定を可視化する
    if (mIsRenderCollision) {
        renderCollision();
    }
}

void AABBAnimationCollider::finalize() {
    Collider::finalize();

    if (mPhysics) {
        //mPhysics->remove(shared_from_this());
    }
}

void AABBAnimationCollider::onEnable(bool value) {
    Collider::onEnable(value);

    setRenderCollision(value);
}

void AABBAnimationCollider::drawInspector() {
    Collider::drawInspector();

    ImGui::Checkbox("IsRenderCollision", &mIsRenderCollision);
}

void AABBAnimationCollider::concatenate(unsigned a, unsigned b) {
    auto size = mAABBs.size();
    assert(a < size);
    assert(b < size);

    mAABBs[a].concatenateTargets.emplace_back(b);
    mAABBs[a].isActive = true;
    mAABBs[b].isActive = false;
}

const AABB& AABBAnimationCollider::getAABB(unsigned index) const {
    assert(index < mAABBs.size());
    return mAABBs[index].aabb;
}

void AABBAnimationCollider::setRenderCollision(bool value) {
    mIsRenderCollision = value;
}

void AABBAnimationCollider::computeAABB() {
    //すべてのメッシュからAABBを作成する
    for (size_t i = 0; i < mAABBs.size(); ++i) {
        const auto& target = mAABBs[i];
        if (!target.isActive) {
            continue;
        }
        resizeAABB(i);

        const auto& targets = target.concatenateTargets;
        for (const auto& t : targets) {
            updateAABB(i, t);
        }
    }
}

void AABBAnimationCollider::updateAABB(unsigned target, unsigned index) {
    //スキニング結果から更新する
    Vector3 min, max;
    computeMinMax(min, max, mAnimationCPU->getCurrentMotionVertexPositions(index));

    auto& aabb = mAABBs[target].aabb;
    aabb.updateMinMax(min);
    aabb.updateMinMax(max);
}

void AABBAnimationCollider::resizeAABB(unsigned index) {
    //スキニング結果から更新する
    Vector3 min, max;
    computeMinMax(min, max, mAnimationCPU->getCurrentMotionVertexPositions(index));

    auto& aabb = mAABBs[index].aabb;
    aabb.min = min;
    aabb.max = max;
}

void AABBAnimationCollider::createAABB() {
    //メッシュの数分拡張する
    unsigned meshCount = mMesh->getMesh()->getMeshCount();
    mAABBs.resize(meshCount);
    //すべてのメッシュからAABBを作成する
    for (unsigned i = 0; i < meshCount; ++i) {
        resizeAABB(i);
    }
}

void AABBAnimationCollider::computeMinMax(Vector3& outMin, Vector3& outMax, const MeshVertexPositions& positions) {
    auto min = Vector3::one * Math::infinity;
    auto max = Vector3::one * Math::negInfinity;

    //メッシュ情報から最小、最大点を割り出す
    for (const auto& p : positions) {
        if (p.x < min.x) {
            min.x = p.x;
        }
        if (p.x > max.x) {
            max.x = p.x;
        }
        if (p.y < min.y) {
            min.y = p.y;
        }
        if (p.y > max.y) {
            max.y = p.y;
        }
        if (p.z < min.z) {
            min.z = p.z;
        }
        if (p.z > max.z) {
            max.z = p.z;
        }
    }

    outMin = min;
    outMax = max;
}

void AABBAnimationCollider::updatePoints() {
    for (auto&& target : mAABBs) {
        if (!target.isActive) {
            continue;
        }

        const auto& aabb = target.aabb;
        const auto& min = aabb.min;
        const auto& max = aabb.max;

        auto& points = target.points;
        points[BoxConstantGroup::BOX_NEAR_BOTTOM_LEFT] = min;
        points[BoxConstantGroup::BOX_NEAR_BOTTOM_RIGHT] = Vector3(max.x, min.y, min.z);
        points[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT] = Vector3(min.x, min.y, max.z);
        points[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT] = Vector3(max.x, min.y, max.z);
        points[BoxConstantGroup::BOX_NEAR_TOP_LEFT] = Vector3(min.x, max.y, min.z);
        points[BoxConstantGroup::BOX_NEAR_TOP_RIGHT] = Vector3(max.x, max.y, min.z);
        points[BoxConstantGroup::BOX_BACK_TOP_LEFT] = Vector3(min.x, max.y, max.z);
        points[BoxConstantGroup::BOX_BACK_TOP_RIGHT] = max;
    }
}

void AABBAnimationCollider::renderCollision() {
#ifdef _DEBUG
    //デバッグ時のみ当たり判定を表示
    for (const auto& target : mAABBs) {
        if (!target.isActive) {
            continue;
        }

        const auto& points = target.points;
        Debug::renderLine(points[BoxConstantGroup::BOX_NEAR_BOTTOM_LEFT], points[BoxConstantGroup::BOX_NEAR_BOTTOM_RIGHT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_NEAR_BOTTOM_LEFT], points[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT], points[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_NEAR_BOTTOM_RIGHT], points[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT], ColorPalette::lightGreen);

        Debug::renderLine(points[BoxConstantGroup::BOX_NEAR_TOP_LEFT], points[BoxConstantGroup::BOX_NEAR_TOP_RIGHT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_NEAR_TOP_LEFT], points[BoxConstantGroup::BOX_BACK_TOP_LEFT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_BACK_TOP_LEFT], points[BoxConstantGroup::BOX_BACK_TOP_RIGHT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_NEAR_TOP_RIGHT], points[BoxConstantGroup::BOX_BACK_TOP_RIGHT], ColorPalette::lightGreen);

        Debug::renderLine(points[BoxConstantGroup::BOX_NEAR_BOTTOM_LEFT], points[BoxConstantGroup::BOX_NEAR_TOP_LEFT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_NEAR_BOTTOM_RIGHT], points[BoxConstantGroup::BOX_NEAR_TOP_RIGHT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_BACK_BOTTOM_LEFT], points[BoxConstantGroup::BOX_BACK_TOP_LEFT], ColorPalette::lightGreen);
        Debug::renderLine(points[BoxConstantGroup::BOX_BACK_BOTTOM_RIGHT], points[BoxConstantGroup::BOX_BACK_TOP_RIGHT], ColorPalette::lightGreen);
    }
#endif // _DEBUG
}
