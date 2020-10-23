#include "AABBCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../Mesh/IMesh.h"
#include "../../Transform/Transform3D.h"

AABBCollider::AABBCollider(GameObject& gameObject) :
    Collider(gameObject),
    mAABB(),
    mDefaultMin(Vector3::zero),
    mDefaultMax(Vector3::one) {
}

AABBCollider::~AABBCollider() = default;

void AABBCollider::start() {
    Collider::start();

    auto meshComponent = getComponent<MeshComponent>();
    if (meshComponent) {
        const auto& mesh = meshComponent->getMesh();
        const auto& meshesVertices = mesh.getMeshesVertices();
        computeBox(meshesVertices);
        mAABB.min = mDefaultMin;
        mAABB.max = mDefaultMax;

        //早速transformが変わっているかもしれないから更新する
        updateAABB();
    }
}

void AABBCollider::onUpdateWorldTransform() {
    Collider::onUpdateWorldTransform();

    if (!mIsAutoUpdate) {
        return;
    }

    updateAABB();
}

void AABBCollider::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    Collider::drawDebugInfo(inspect);

    inspect->emplace_back("Min", mAABB.min);
    inspect->emplace_back("Max", mAABB.max);
}

void AABBCollider::set(const Vector3& min, const Vector3& max) {
    mAABB.min = min;
    mAABB.max = max;
    if (mIsAutoUpdate) {
        mIsAutoUpdate = false;
    }
}

const AABB& AABBCollider::getAABB() const {
    return mAABB;
}

void AABBCollider::computeBox(const std::vector<MeshVertices>& meshesVertices) {
    auto min = Vector3::one * Math::infinity;
    auto max = Vector3::one * Math::negInfinity;

    //すべてのメッシュ情報から最小、最大点を割り出す
    for (size_t i = 0; i < meshesVertices.size(); ++i) {
        const auto& vertices = meshesVertices[i];
        for (size_t j = 0; j < vertices.size(); ++j) {
            const auto& p = vertices[j].pos;
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
    }

    mDefaultMin = min;
    mDefaultMax = max;
}

void AABBCollider::updateAABB() {
    const auto& t = transform();
    const auto& pos = t.getPosition();
    const auto& scale = t.getScale();
    mAABB.min = mDefaultMin * scale + pos;
    mAABB.max = mDefaultMax * scale + pos;
    mAABB.rotate(transform().getRotation());
}
