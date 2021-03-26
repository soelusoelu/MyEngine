#include "AABBAnimationCollider.h"
#include "../Mesh/AnimationCPU.h"
#include "../Mesh/MeshComponent.h"
#include "../../../Device/Physics.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"

AABBAnimationCollider::AABBAnimationCollider(GameObject& gameObject)
    : Collider(gameObject)
    , mAABBs()
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

    unsigned meshCount = mMesh->getMesh()->getMeshCount();
    mAABBs.resize(meshCount);
    mPoints.resize(meshCount);

    //最新のAABBの点を計算する
    updatePoints();

    if (mPhysics) {
        //mPhysics->add(shared_from_this());
    }
}

void AABBAnimationCollider::lateUpdate() {
    Collider::lateUpdate();

    //AABBを更新する
    updateAABB();

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

const std::vector<AABB>& AABBAnimationCollider::getAABBs() const {
    return mAABBs;
}

void AABBAnimationCollider::setRenderCollision(bool value) {
    mIsRenderCollision = value;
}

void AABBAnimationCollider::updateAABB() {
    //すべてのメッシュからAABBを作成する
    const auto mesh = mMesh->getMesh();
    for (size_t i = 0; i < mesh->getMeshCount(); ++i) {
        Vector3 min, max;
        computeMinMax(min, max, mAnimationCPU->getCurrentMotionVertexPositions(i));

        mAABBs[i].min = min;
        mAABBs[i].max = max;
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
    for (size_t i = 0; i < mAABBs.size(); ++i) {
        const auto& min = mAABBs[i].min;
        const auto& max = mAABBs[i].max;

        auto& points = mPoints[i];
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
    for (const auto& points : mPoints) {
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
