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

const AssetsRenderTexture& AssetsRenderTextureManager::getCurrentSelectTexture() const {
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

void AssetsRenderTextureManager::initialize(const std::shared_ptr<Camera>& camera, IInspectorTargetSetter* inspector, const IMeshesGetter* getter) {
    mTextureList->initialize();
    mTextureAdder->initialize(mTextureList.get());
    mDeleter->initialize(mTextureList.get(), this);
    mSelector->initialize(mTextureList.get());
    mPlacement->initialize(camera, inspector, getter, this);
}

void AssetsRenderTextureManager::update() {
    mSelector->selectTexture(mCurrentSelectTexture);
    mTextureList->update();
    mTextureAdder->update();
    mDeleter->update();
    mPlacement->update();
}

void AssetsRenderTextureManager::drawMeshes() const {
    mTextureList->drawMeshOnTexture();
}

void AssetsRenderTextureManager::drawTextures(const Matrix4& proj) const {
    mTextureList->drawTexture(proj);
    mTextureAdder->draw(proj);
}
