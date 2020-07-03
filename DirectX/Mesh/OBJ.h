#pragma once

#include "IMeshLoader.h"
#include "../Math/Math.h"
#include <fstream>
#include <memory>
#include <string>
#include <vector>

struct Material;
class VertexArray;

class OBJ : public IMeshLoader {
public:
    OBJ();
    ~OBJ();
    virtual void perse(const std::string& fileName) override;
    virtual void setInitMaterials(MaterialPtrArray* rhs) const override;
    virtual std::shared_ptr<VertexArray> getVertexArray() const override;
    virtual Vector3 getCenter() const override;
    virtual float getRadius() const override;

private:
    bool preload(std::ifstream& stream, const std::string& fileName); //事前に頂点数などを調べる
    bool materialLoad(const std::string& materialName);
    void materialPreload(std::ifstream& stream);

private:
    MaterialPtrArray mInitMaterials;
    std::shared_ptr<VertexArray> mVertexArray;
    MeshVertex* mVertices;
};
