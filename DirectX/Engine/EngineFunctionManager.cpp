#include "EngineFunctionManager.h"
#include "AssetsRenderer/AssetsRenderTextureManager.h"
#include "DebugManager/DebugManager.h"
#include "DebugManager/DebugLayer/DebugLayer.h"
#include "Pause/Pause.h"
#include "SceneMesh/SceneMeshOperator.h"
#include "../Device/Renderer.h"

EngineFunctionManager::EngineFunctionManager()
    : mDebugManager(std::make_unique<DebugManager>())
    , mPause(std::make_unique<Pause>())
    , mAssetsRenderTextureManager(std::make_unique<AssetsRenderTextureManager>())
    , mSceneMeshOperator(std::make_unique<SceneMeshOperator>())
    , mMode(DebugMode::LOG)
{
}

EngineFunctionManager::~EngineFunctionManager() = default;

void EngineFunctionManager::loadProperties(const rapidjson::Value& inObj) {
    mDebugManager->loadProperties(inObj);
    mPause->loadProperties(inObj);
    mAssetsRenderTextureManager->loadProperties(inObj);
}

void EngineFunctionManager::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) {
    mDebugManager->saveProperties(alloc, inObj);
    mPause->saveProperties(alloc, inObj);
    mAssetsRenderTextureManager->saveProperties(alloc, inObj);
}

void EngineFunctionManager::initialize(const std::shared_ptr<Camera>& camera, const IGameObjectsGetter* gameObjctsGetter, const IMeshesGetter* meshesGetter, const IFpsGetter* fpsGetter) {
    mDebugManager->initialize(gameObjctsGetter, fpsGetter, mPause.get());
    mPause->initialize();
    mAssetsRenderTextureManager->initialize(camera, mDebugManager->getDebugLayer().inspector(), meshesGetter);
    mSceneMeshOperator->initialize(camera, meshesGetter);
}

void EngineFunctionManager::preUpdateProcess() {
    mDebugManager->preUpdateProcess();
}

void EngineFunctionManager::update() {
    mDebugManager->update();
    mPause->update();
    mAssetsRenderTextureManager->update();
    mSceneMeshOperator->update();
}

void EngineFunctionManager::changeScene(const std::string& scene) {
    if (scene == DebugSceneName::MAP_EDITOR) {
        mMode = DebugMode::MESH_TEXTURES;
    } else {
        mMode = DebugMode::LOG;
    }
}

void EngineFunctionManager::draw(const Renderer& renderer, Matrix4& proj) const {
    //レンダリング領域をデバッグに変更
    renderer.renderToDebug(proj);

    if (mMode == DebugMode::MESH_TEXTURES) {
        mAssetsRenderTextureManager->drawTextures(proj);
    }

    mPause->drawButton(proj);
    mDebugManager->draw(mMode, renderer, proj);
}

void EngineFunctionManager::draw3D(const Renderer& renderer, const Matrix4& viewProj) const {
    mAssetsRenderTextureManager->drawMeshes();
    mDebugManager->draw3D(renderer, viewProj);
}

DebugManager& EngineFunctionManager::debug() const {
    return *mDebugManager;
}

IPause& EngineFunctionManager::pause() const {
    return *mPause;
}

AssetsRenderTextureManager& EngineFunctionManager::getAssetsRenderTextureManager() const {
    return *mAssetsRenderTextureManager;
}
