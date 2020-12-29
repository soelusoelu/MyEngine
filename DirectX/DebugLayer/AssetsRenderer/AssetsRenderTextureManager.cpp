#include "AssetsRenderTextureManager.h"
#include "AssetsPlacement.h"
#include "AssetsRenderTextureAdder.h"
#include "AssetsRenderTextureDeleter.h"
#include "AssetsRenderTextureList.h"
#include "AssetsTexturesSelector.h"

AssetsRenderTextureManager::AssetsRenderTextureManager()
    : mTextureList(std::make_unique<AssetsRenderTextureList>())
    , mTextureAdder(std::make_unique<AssetsRenderTextureAdder>())
    , mDeleter(std::make_unique<AssetsRenderTextureDeleter>())
    , mSelector(std::make_unique<AssetsTexturesSelector>())
    , mPlacement(std::make_unique<AssetsPlacement>())
    , mCurrentSelectTexture(nullptr)
{
}

AssetsRenderTextureManager::~AssetsRenderTextureManager() = default;

const MeshRenderOnTexture& AssetsRenderTextureManager::getCurrentSelectTexture() const {
    return *mCurrentSelectTexture;
}

bool AssetsRenderTextureManager::selectedTexture() const {
    return (mCurrentSelectTexture) ? true : false;
}

void AssetsRenderTextureManager::loadProperties(const rapidjson::Value& inObj) {
    mTextureList->loadProperties(inObj);
    mTextureAdder->loadProperties(inObj);
}

void AssetsRenderTextureManager::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    mTextureList->saveProperties(alloc, inObj);
    mTextureAdder->saveProperties(alloc, inObj);
}

void AssetsRenderTextureManager::initialize() {
    mTextureList->initialize();
    mTextureAdder->initialize(mTextureList.get());
    mDeleter->initialize(mTextureList.get(), this);
    mSelector->initialize(mTextureList.get());
    mPlacement->initialize(this);
}

void AssetsRenderTextureManager::update() {
    mSelector->selectTexture(mCurrentSelectTexture);
    mTextureList->update();
    mTextureAdder->update();
    mDeleter->update();
    mPlacement->update();
}

void AssetsRenderTextureManager::drawMeshes() {
    mTextureList->drawMeshOnTexture();
}

void AssetsRenderTextureManager::drawTextures(const Matrix4& proj) {
    mTextureList->drawTexture(proj);
    mTextureAdder->draw(proj);
}

AssetsPlacement& AssetsRenderTextureManager::getAssetsPlacement() const {
    return *mPlacement;
}
