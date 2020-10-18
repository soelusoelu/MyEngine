#pragma once

#include "IMeshLoader.h"
#include "../Math/Math.h"
#include <fbxsdk.h>
#include <memory>
#include <string>
#include <vector>

class FBX : public IMeshLoader {
public:
    FBX();
    ~FBX();
    virtual void perse(const std::string& fileName, std::vector<MeshVertex>& vertices) override;
    virtual const std::vector<Vector3>& getPositions() const override;
    virtual const std::vector<Vector3>& getNormals() const override;
    virtual const std::vector<Vector2>& getUVs() const override;
    virtual const std::vector<unsigned short>& getIndices() const override;
    virtual const Material& getMaterial(unsigned index = 0) const override;
    virtual bool isUseMaterial() const override;

private:
    void createMesh(std::vector<MeshVertex>& vertices, FbxMesh* mesh);
    void loadPosition(FbxMesh* mesh);
    void loadNormal(FbxMesh* mesh);
    void loadUV(FbxMesh* mesh);
    void loadFace(std::vector<MeshVertex>& vertices, FbxMesh* mesh);

    void loadMaterial(FbxMesh* mesh);
    void loadMaterialAttribute(FbxSurfaceMaterial* material);
    void loadMaterialTexture(FbxSurfaceMaterial* material);

private:
    std::vector<Vector3> mPositions;
    std::vector<Vector3> mNormals;
    std::vector<Vector2> mUVs;
    std::vector<unsigned short> mIndices;
    std::vector<Material> mMaterials;
};
