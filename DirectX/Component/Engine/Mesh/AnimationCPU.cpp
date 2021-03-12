#include "AnimationCPU.h"
#include "MeshComponent.h"
#include "SkinMeshComponent.h"
#include "../../../System/SystemInclude.h"
#include <cassert>

AnimationCPU::AnimationCPU(GameObject& gameObject)
    : Component(gameObject)
    , mMesh(nullptr)
    , mAnimation(nullptr)
{
}

AnimationCPU::~AnimationCPU() = default;

void AnimationCPU::start() {
    auto meshComp = getComponent<MeshComponent>();
    mMesh = meshComp->getMesh();

    mAnimation = getComponent<SkinMeshComponent>();
    mAnimation->callbackComputeCurrentBones([&] { updateVertexPositionsAfterSkinning(); });

    auto meshCount = mMesh->getMeshCount();
    mCurrentMeshesVertexPositions.resize(meshCount);
    for (size_t i = 0; i < meshCount; ++i) {
        mCurrentMeshesVertexPositions[i].resize(mMesh->getMeshVertices(i).size());
    }
}

const MeshVertexPositions& AnimationCPU::getCurrentMotionVertexPositions(unsigned index) const {
    assert(index < mMesh->getMeshCount());
    return mCurrentMeshesVertexPositions[index];
}

void AnimationCPU::updateVertexPositionsAfterSkinning() {
    for (size_t i = 0; i < mMesh->getMeshCount(); ++i) {
        auto& vertexPositions = mCurrentMeshesVertexPositions[i];
        const auto& vertices = mMesh->getMeshVertices(i);
        for (size_t j = 0; j < vertices.size(); ++j) {
            Matrix4 comb{};
            ZeroMemory(&comb, sizeof(comb));
            computeCombinationMatrix(comb, vertices[j]);

            vertexPositions[j] = Vector3::transform(vertices[j].pos, comb);
        }
    }
}

void AnimationCPU::computeCombinationMatrix(Matrix4& out, const MeshVertex& vertex) {
    for (int i = 0; i < 4; ++i) {
        const auto& currentBones = mAnimation->getBoneCurrentFrameMatrix();
        const auto& bone = currentBones[vertex.index[i]];
        float weigth = vertex.weight[i];
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                out.m[j][k] += bone.m[j][k] * weigth;
            }
        }
    }
}
