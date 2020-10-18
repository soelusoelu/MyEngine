#pragma once

#include "../Math/Math.h"
#include "Material.h"
#include <memory>
#include <string>
#include <vector>

//頂点の構造体
struct MeshVertex {
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;
};

class IMeshLoader {
public:
    virtual ~IMeshLoader() = default;
    virtual void perse(const std::string& fileName, std::vector<MeshVertex>& vertices) = 0;
    virtual const std::vector<Vector3>& getPositions() const = 0;
    virtual const std::vector<Vector3>& getNormals() const = 0;
    virtual const std::vector<Vector2>& getUVs() const = 0;
    virtual const std::vector<unsigned short>& getIndices() const = 0;
    virtual const Material& getMaterial(unsigned index = 0) const = 0;
    virtual bool isUseMaterial() const = 0;
    virtual Vector3 getCenter() const = 0;
    virtual float getRadius() const = 0;
};
