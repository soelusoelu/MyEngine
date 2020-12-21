#include "AssetsRenderTextureManager.h"
#include "AssetsRenderTextureAdder.h"
#include "AssetsRenderTextureList.h"

AssetsRenderTextureManager::AssetsRenderTextureManager()
    : mTextureList(std::make_unique<AssetsRenderTextureList>())
    , mTextureAdder(std::make_unique<AssetsRenderTextureAdder>())
{
}

AssetsRenderTextureManager::~AssetsRenderTextureManager() = default;

void AssetsRenderTextureManager::loadProperties(const rapidjson::Value& inObj) {
    mTextureList->loadProperties(inObj);
    mTextureAdder->loadProperties(inObj);
}

void AssetsRenderTextureManager::initialize() {
    mTextureAdder->initialize(mTextureList.get());
}

void AssetsRenderTextureManager::update() {
    mTextureList->update();
    mTextureAdder->update();
}

void AssetsRenderTextureManager::drawMeshes() {
    mTextureList->drawMeshOnTexture();
}

void AssetsRenderTextureManager::drawTextures(const Matrix4& proj) {
    mTextureList->drawTexture(proj);
    mTextureAdder->draw(proj);
}
