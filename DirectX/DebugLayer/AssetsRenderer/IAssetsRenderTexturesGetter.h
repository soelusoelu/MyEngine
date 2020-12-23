#pragma once

#include <list>
#include <memory>

class MeshRenderOnTexture;

using MeshRenderOnTexturePtr = std::shared_ptr<MeshRenderOnTexture>;
using MeshRenderOnTexturePtrList = std::list<MeshRenderOnTexturePtr>;

class IAssetsRenderTexturesGetter {
public:
    virtual ~IAssetsRenderTexturesGetter() = default;
    virtual const MeshRenderOnTexturePtrList& getTextures() const = 0;
};