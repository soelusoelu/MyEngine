//#pragma once
//
//#include "IMeshLoader.h"
//#include "../Math/Math.h"
//#include <fbxsdk.h>
//#include <memory>
//#include <string>
//#include <vector>
//
//class VertexArray;
//
//class FBX : public IMeshLoader {
//public:
//    FBX();
//    ~FBX();
//    virtual void perse(const std::string& fileName) override;
//    virtual void setInitMaterials(MaterialPtrArray* rhs) const override;
//    virtual std::shared_ptr<VertexArray> getVertexArray() const override;
//    virtual Vector3 getCenter() const override;
//    virtual float getRadius() const override;
//
//private:
//    void perse(FbxNode* node, int indent);
//    void getIndex(FbxMesh* mesh);
//    void getVertex(FbxMesh* mesh);
//    void getNormals(FbxMesh* mesh);
//    void getUV(FbxMesh* mesh);
//    void getMaterial(FbxMesh* mesh);
//
//private:
//    std::vector<unsigned> mIndexArray;
//    std::vector<MeshVertex> mVertices;
//    MaterialPtrArray mInitMaterials;
//    std::shared_ptr<VertexArray> mVertexArray;
//};
