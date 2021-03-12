#pragma once

#include "../../../Math/Math.h"
#include <vector>

//頂点位置の配列を別名定義
using MeshVertexPositions = std::vector<Vector3>;
using MeshesVertexPositions = std::vector<MeshVertexPositions>;

//アニメーションCPU用インターフェース
class IAnimationCPU {
public:
    virtual ~IAnimationCPU() = default;
    //モーション後の頂点位置の配列を取得する
    virtual const MeshVertexPositions& getCurrentMotionVertexPositions(unsigned index) const = 0;
};
