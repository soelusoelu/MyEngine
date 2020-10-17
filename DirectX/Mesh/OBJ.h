#pragma once

#include "IMeshLoader.h"
#include "Material.h"
#include "../Math/Math.h"
#include <fstream>
#include <memory>
#include <string>
#include <vector>

class OBJ : public IMeshLoader {
public:
    OBJ();
    ~OBJ();
    virtual void perse(const std::string& fileName, std::vector<MeshVertex>& vertices) override;
    virtual const std::vector<Vector3>& getPositions() const override;
    virtual const std::vector<Vector3>& getNormals() const override;
    virtual const std::vector<Vector2>& getUVs() const override;
    virtual const std::vector<unsigned short>& getIndices() const override;
    virtual Vector3 getCenter() const override;
    virtual float getRadius() const override;

private:
    bool materialLoad(const std::string& materialName);

private:
    Material mMaterial;

    std::vector<Vector3> mPositions;
    std::vector<Vector3> mNormals;
    std::vector<Vector2> mUVs;
    std::vector<unsigned short> mIndices;
};
