#pragma once

class MeshRenderOnTexture;

class ICurrentSelectTextureGetter {
public:
    virtual ~ICurrentSelectTextureGetter() = default;
    virtual const MeshRenderOnTexture& getCurrentSelectTexture() const = 0;
    virtual bool selectedTexture() const = 0;
};
