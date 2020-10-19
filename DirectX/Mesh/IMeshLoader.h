#pragma once

#include "../Math/Math.h"
#include "Material.h"
#include <string>
#include <vector>

//頂点の構造体
struct MeshVertex {
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;
};

//頂点構造体の配列を別名定義
using MeshVertices = std::vector<MeshVertex>;

class IMeshLoader {
public:
    virtual ~IMeshLoader() = default;
    virtual void perse(const std::string& fileName, std::vector<MeshVertices>& meshes) = 0;
    virtual const std::vector<unsigned short>& getIndices(unsigned meshIndex) const = 0;
    virtual const Material& getMaterial(unsigned index) const = 0;
    virtual unsigned getMeshCount() const = 0;
};
