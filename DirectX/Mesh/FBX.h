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
    virtual void perse(const std::string& fileName, std::vector<MeshVertices>& meshes) override;
    virtual const std::vector<unsigned short>& getIndices(unsigned meshIndex) const override;
    virtual const Material& getMaterial(unsigned index) const override;
    virtual unsigned getMeshCount() const override;

private:
    void createMesh(MeshVertices& meshVertices, FbxMesh* mesh, unsigned meshIndex);
    void loadPosition(FbxMesh* mesh, unsigned meshIndex);
    void loadNormal(FbxMesh* mesh, unsigned meshIndex);
    void loadUV(FbxMesh* mesh, unsigned meshIndex);
    void loadFace(MeshVertices& meshVertices, FbxMesh* mesh, unsigned meshIndex);
    void computeIndices(FbxMesh* mesh, unsigned meshIndex);

    void loadMaterial(FbxMesh* mesh, unsigned meshIndex);
    void loadMaterialAttribute(FbxSurfaceMaterial* material, unsigned meshIndex);
    void loadMaterialTexture(FbxSurfaceMaterial* material, unsigned meshIndex);

private:
    unsigned mNumMeshes;

    std::vector<std::vector<unsigned short>> mIndices;
    std::vector<Material> mMaterials;
};
