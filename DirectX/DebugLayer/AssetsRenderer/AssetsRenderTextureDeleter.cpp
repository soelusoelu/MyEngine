#include "AssetsRenderTextureDeleter.h"
#include "../../System/Texture/MeshRenderOnTexture.h"
#include "../../Input/Input.h"

AssetsRenderTextureDeleter::AssetsRenderTextureDeleter()
    : mDeleter(nullptr)
    , mCurrnetTextureGetter(nullptr)
{
}

AssetsRenderTextureDeleter::~AssetsRenderTextureDeleter() = default;

void AssetsRenderTextureDeleter::initialize(IAssetsRenderTextureDeleter* deleter, const ICurrentSelectTextureGetter* getter) {
    mDeleter = deleter;
    mCurrnetTextureGetter = getter;
}

void AssetsRenderTextureDeleter::update() {
    if (Input::keyboard().getKeyDown(KeyCode::Delete)) {
        mDeleter->deleteTexture(mCurrnetTextureGetter->getCurrentSelectTexture().getFilePath());
    }
}
