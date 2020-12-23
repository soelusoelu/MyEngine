#include "AssetsPlacement.h"
#include "AssetsTexturesSelector.h"
#include "../../Input/Input.h"
#include "../../System/Texture/MeshRenderOnTexture.h"

AssetsPlacement::AssetsPlacement()
    : mSelector(std::make_unique<AssetsTexturesSelector>())
    , mCurrentSelectTexture(nullptr)
{
}

AssetsPlacement::~AssetsPlacement() = default;

void AssetsPlacement::initialize(const IAssetsRenderTexturesGetter* getter) {
    mSelector->initialize(getter);
}

void AssetsPlacement::update() {
    mSelector->selectTexture(mCurrentSelectTexture);
}
