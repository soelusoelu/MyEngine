#include "AnimationCPU.h"
#include "MeshComponent.h"
#include "SkinMeshComponent.h"
#include "../../../System/SystemInclude.h"
#include <cassert>

AnimationCPU::AnimationCPU(GameObject& gameObject)
    : Component(gameObject)
    , mMesh(nullptr)
    , mAnimation(nullptr)
    , mCurrentFrame(0)
{
}

AnimationCPU::~AnimationCPU() = default;

void AnimationCPU::start() {
    auto meshComp = getComponent<MeshComponent>();
    mMesh = meshComp->getMesh();
    mAnimation = meshComp->getAnimation();
    mCurrentBones.resize(mAnimation->getBoneCount());
    auto meshCount = mMesh->getMeshCount();
    mCurrentMeshesVertexPositions.resize(meshCount);
    for (size_t i = 0; i < meshCount; ++i) {
        mCurrentMeshesVertexPositions[i].resize(mMesh->getMeshVertices(i).size());
    }
}

void AnimationCPU::update() {
    const auto& motion = mAnimation->getMotion(0);
    ++mCurrentFrame;
    if (mCurrentFrame >= motion.numFrame) {
        mCurrentFrame = 0;
    }

    for (size_t i = 0; i < mAnimation->getBoneCount(); ++i) {
        mCurrentBones[i] = mAnimation->getBone(i).offsetMat * motion.frameMat[i][mCurrentFrame];
    }


    //ここから大事
    for (size_t i = 0; i < mMesh->getMeshCount(); ++i) {
        auto& vertexPositions = mCurrentMeshesVertexPositions[i];
        const auto& vertices = mMesh->getMeshVertices(i);
        for (size_t j = 0; j < vertices.size(); ++j) {
            const auto& vertex = vertices[j];
            Matrix4 comb;
            ZeroMemory(&comb, sizeof(comb));
            computeCombinationMatrix(comb, vertex);

            vertexPositions[j] = Vector3::transform(vertices[j].pos, comb);
        }
    }
}

const MeshVertexPositions& AnimationCPU::getCurrentMotionVertexPositions(unsigned index) const {
    assert(index < mMesh->getMeshCount());
    return mCurrentMeshesVertexPositions[index];
}

void AnimationCPU::computeCombinationMatrix(Matrix4& out, const MeshVertex& vertex) {
    for (int i = 0; i < 4; ++i) {
        const auto& bone = mCurrentBones[vertex.index[i]];
        float weigth = vertex.weight[i];
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                out.m[j][k] += bone.m[j][k] * weigth;
            }
        }
    }
}
