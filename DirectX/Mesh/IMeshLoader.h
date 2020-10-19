#pragma once

#include "../Math/Math.h"
#include "Material.h"
#include <string>
#include <vector>

//一つのメッシュが持つ情報
struct MeshParam {
    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> uvs;
};

//頂点の構造体
struct MeshVertex {
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;
};

class IMeshLoader {
public:
    virtual ~IMeshLoader() = default;
    virtual void perse(const std::string& fileName, std::vector<MeshParam>& meshes) = 0;
    virtual const std::vector<Vector3>& getPositions(unsigned meshIndex) const = 0;
    virtual const std::vector<unsigned short>& getIndices(unsigned meshIndex) const = 0;
    virtual const Material& getMaterial(unsigned index) const = 0;
    virtual unsigned getMeshCount() const = 0;
};
