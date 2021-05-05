#pragma once

#include "IAnimationCPU.h"
#include "../../Component.h"
#include "../../../Math/Math.h"
#include "../../../Mesh/IMesh.h"
#include <memory>

class SkinMeshComponent;

//頂点ブレンドをCPU(このクラス)で行うクラス
class AnimationCPU
    : public Component
    , public IAnimationCPU
{
public:
    AnimationCPU();
    ~AnimationCPU();
    virtual void start() override;

    virtual const MeshVertexPositions& getCurrentMotionVertexPositions(unsigned index) const override;

private:
    AnimationCPU(const AnimationCPU&) = delete;
    AnimationCPU& operator=(const AnimationCPU&) = delete;

    //スキニング後の頂点位置を計算求める
    void updateVertexPositionsAfterSkinning();
    //合成用行列を計算する
    void computeCombinationMatrix(Matrix4& out, const MeshVertex& vertex);

private:
    IMesh* mMesh;
    std::shared_ptr<SkinMeshComponent> mAnimation;
    //モーション中の頂点位置配列
    MeshesVertexPositions mCurrentMeshesVertexPositions;
    //モーション中のメッシュ頂点情報配列
    std::vector<MeshVertices> mCurrentMeshesVertices;
};
