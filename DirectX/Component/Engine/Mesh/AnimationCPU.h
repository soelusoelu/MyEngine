#pragma once

#include "IAnimationCPU.h"
#include "../../Component.h"
#include "../../../Math/Math.h"
#include "../../../Mesh/IAnimation.h"
#include "../../../Mesh/IMesh.h"
#include <vector>

//頂点ブレンドをCPU(このクラス)で行うアニメーションクラス
class AnimationCPU
    : public Component
    , public IAnimationCPU {

public:
    AnimationCPU(GameObject& gameObject);
    ~AnimationCPU();
    virtual void start() override;
    virtual void update() override;

    virtual const MeshVertexPositions& getCurrentMotionVertexPositions(unsigned index) const override;

private:
    AnimationCPU(const AnimationCPU&) = delete;
    AnimationCPU& operator=(const AnimationCPU&) = delete;

    void computeCombinationMatrix(Matrix4& out, const MeshVertex& vertex);

private:
    const IMesh* mMesh;
    const IAnimation* mAnimation;
    std::vector<Matrix4> mCurrentBones;
    MeshesVertexPositions mCurrentMeshesVertexPositions;
    int mCurrentFrame;
};
