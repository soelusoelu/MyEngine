#include "AssetsRenderTextureManager.h"
#include "AssetsPlacement.h"
#include "AssetsRenderTextureAdder.h"
#include "AssetsRenderTextureList.h"

AssetsRenderTextureManager::AssetsRenderTextureManager()
    : mTextureList(std::make_unique<AssetsRenderTextureList>())
    , mTextureAdder(std::make_unique<AssetsRenderTextureAdder>())
    , mPlanement(std::make_unique<AssetsPlacement>())
{
}

AssetsRenderTextureManager::~AssetsRenderTextureManager() = default;

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
    mPlanement->initialize(mTextureList.get());
}

void AssetsRenderTextureManager::update() {
    mTextureList->update();
    mTextureAdder->update();
    mPlanement->update();
}

void AssetsRenderTextureManager::drawMeshes() {
    mTextureList->drawMeshOnTexture();
}

void AssetsRenderTextureManager::drawTextures(const Matrix4& proj) {
    mTextureList->drawTexture(proj);
    mTextureAdder->draw(proj);
}
