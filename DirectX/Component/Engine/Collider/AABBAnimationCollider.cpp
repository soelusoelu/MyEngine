#include "AABBAnimationCollider.h"
#include "../Mesh/SkinMeshComponent.h"
#include "../../../Device/Physics.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"

AABBAnimationCollider::AABBAnimationCollider(GameObject& gameObject)
    : Collider(gameObject)
    , mAABB()
    , mAnimation(nullptr)
    , mIsRenderCollision(true)
{
}

AABBAnimationCollider::~AABBAnimationCollider() = default;

void AABBAnimationCollider::start() {
    Collider::start();

    mAnimation = getComponent<SkinMeshComponent>();

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

const AABB& AABBAnimationCollider::getAABB() const {
    return mAABB;
}

void AABBAnimationCollider::setRenderCollision(bool value) {
    mIsRenderCollision = value;
}

void AABBAnimationCollider::updateAABB() {

}

void AABBAnimationCollider::updatePoints() {
    const auto& min = mAABB.min;
    const auto& max = mAABB.max;
    mPoints[0] = min;
    mPoints[1] = Vector3(max.x, min.y, min.z);
    mPoints[2] = Vector3(min.x, min.y, max.z);
    mPoints[3] = Vector3(max.x, min.y, max.z);
    mPoints[4] = Vector3(min.x, max.y, min.z);
    mPoints[5] = Vector3(max.x, max.y, min.z);
    mPoints[6] = Vector3(min.x, max.y, max.z);
    mPoints[7] = max;
}

void AABBAnimationCollider::renderCollision() {
#ifdef _DEBUG
    //デバッグ時のみ当たり判定を表示
    Debug::renderLine(mPoints[0], mPoints[1], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[0], mPoints[2], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[2], mPoints[3], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[1], mPoints[3], ColorPalette::lightGreen);

    Debug::renderLine(mPoints[4], mPoints[5], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[4], mPoints[6], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[6], mPoints[7], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[5], mPoints[7], ColorPalette::lightGreen);

    Debug::renderLine(mPoints[0], mPoints[4], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[1], mPoints[5], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[2], mPoints[6], ColorPalette::lightGreen);
    Debug::renderLine(mPoints[3], mPoints[7], ColorPalette::lightGreen);
#endif // _DEBUG
}
