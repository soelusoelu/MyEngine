#include "OBBAnimationCollider.h"
#include "../Mesh/AnimationCPU.h"
#include "../Mesh/MeshComponent.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Transform/Transform3D.h"
#include <cassert>

OBBAnimationCollider::OBBAnimationCollider()
    : Collider()
    , mMesh(nullptr)
    , mAnimationCPU(nullptr)
    , mIsRenderCollision(true)
{
}

OBBAnimationCollider::~OBBAnimationCollider() = default;

void OBBAnimationCollider::start() {
    Collider::start();

    mMesh = getComponent<MeshComponent>();
    mAnimationCPU = getComponent<AnimationCPU>();
    if (!mAnimationCPU) {
        mAnimationCPU = addComponent<AnimationCPU>("AnimationCPU");
    }

    //メッシュの数分拡張する
    unsigned meshCount = mMesh->getMesh()->getMeshCount();
    mOBBs.resize(meshCount);
    //すべてのメッシュからOBBを作成する
    for (unsigned i = 0; i < meshCount; ++i) {
        createOBB(i);
    }

    if (mPhysics) {
        //mPhysics->add(shared_from_this());
    }
}

void OBBAnimationCollider::lateUpdate() {
    Collider::lateUpdate();

    //OBBを更新する
    //computeOBB();

    //当たり判定を可視化する
    if (mIsRenderCollision) {
        renderCollision();
    }
}

void OBBAnimationCollider::finalize() {
    Collider::finalize();

    if (mPhysics) {
        //mPhysics->remove(shared_from_this());
    }
}

void OBBAnimationCollider::onEnable(bool value) {
    Collider::onEnable(value);

    setRenderCollision(value);
}

void OBBAnimationCollider::drawInspector() {
    Collider::drawInspector();

    ImGui::Checkbox("IsRenderCollision", &mIsRenderCollision);
}

void OBBAnimationCollider::concatenate(unsigned a, unsigned b) {
    auto size = mOBBs.size();
    assert(a < size);
    assert(b < size);

    auto& obbA = mOBBs[a];
    obbA.concatenateTargets.emplace_back(b);
    obbA.isActive = true;
    mOBBs[b].isActive = false;
}

const OBB& OBBAnimationCollider::getOBB(unsigned index) const {
    assert(index < mOBBs.size());
    return mOBBs[index].obb;
}

void OBBAnimationCollider::setRenderCollision(bool value) {
    mIsRenderCollision = value;
}

void OBBAnimationCollider::computeOBB() {
    //すべてのメッシュからOBBを作成する
    auto size = mOBBs.size();
    for (size_t i = 0; i < size; ++i) {
        const auto& target = mOBBs[i];
        if (!target.isActive) {
            continue;
        }
        createOBB(i);

        const auto& targets = target.concatenateTargets;
        for (const auto& t : targets) {
            updateOBB(i, t);
        }
    }
}

void OBBAnimationCollider::updateOBB(unsigned target, unsigned index) {
    //スキニング結果から更新する
    //Vector3 min, max;
    //computeMinMax(min, max, mAnimationCPU->getCurrentMotionVertexPositions(index));

    //auto& obb = mOBBs[target].obb;
    //obb.updateMinMax(min);
    //obb.updateMinMax(max);
}

void OBBAnimationCollider::createOBB(unsigned meshIndex) {
    const auto& vertices = mMesh->getMesh()->getMeshVertices(meshIndex);
    mOBBs[meshIndex].obb = OBBCreater::create(vertices);
}

void OBBAnimationCollider::renderCollision() {
#ifdef _DEBUG
    //デバッグ時のみ当たり判定を表示
    for (const auto& target : mOBBs) {
        if (!target.isActive) {
            continue;
        }

        const auto& obb = target.obb;
        const auto& edges = obb.edges;
        Vector3 ftr = obb.center + (edges[0] / 2.f) + (edges[1] / 2.f) + (edges[2] / 2.f);
        Vector3 fbr = ftr - edges[2];
        Vector3 ftl = ftr - edges[0];
        Vector3 fbl = ftl - edges[2];

        Vector3 btr = ftr - edges[1];
        Vector3 bbr = btr - edges[2];
        Vector3 btl = btr - edges[0];
        Vector3 bbl = btl - edges[2];

        Debug::renderLine(ftr, fbr, ColorPalette::lightGreen);
        Debug::renderLine(ftr, ftl, ColorPalette::lightGreen);
        Debug::renderLine(ftl, fbl, ColorPalette::lightGreen);
        Debug::renderLine(fbl, fbr, ColorPalette::lightGreen);

        Debug::renderLine(btr, bbr, ColorPalette::lightGreen);
        Debug::renderLine(btr, btl, ColorPalette::lightGreen);
        Debug::renderLine(btl, bbl, ColorPalette::lightGreen);
        Debug::renderLine(bbl, bbr, ColorPalette::lightGreen);

        Debug::renderLine(ftr, btr, ColorPalette::lightGreen);
        Debug::renderLine(ftl, btl, ColorPalette::lightGreen);
        Debug::renderLine(fbr, bbr, ColorPalette::lightGreen);
        Debug::renderLine(fbl, bbl, ColorPalette::lightGreen);
    }
#endif // _DEBUG
}
