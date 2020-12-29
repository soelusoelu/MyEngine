#pragma once

#include <vector>
#include <memory>

class MeshRenderOnTexture;

using MeshRenderOnTexturePtr = std::shared_ptr<MeshRenderOnTexture>;
using MeshRenderOnTexturePtrArray = std::vector<MeshRenderOnTexturePtr>;

class IAssetsRenderTexturesGetter {
public:
    virtual ~IAssetsRenderTexturesGetter() = default;
    virtual const MeshRenderOnTexturePtrArray& getTextures() const = 0;
};