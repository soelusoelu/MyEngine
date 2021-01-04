#include "EngineFunctionManager.h"
#include "AssetsRenderer/AssetsRenderTextureManager.h"
#include "Debug/DebugUtility.h"
#include "Pause/Pause.h"
#include "SceneMesh/SceneMeshOperator.h"
#include "../Device/Renderer.h"

EngineFunctionManager::EngineFunctionManager()
    : mDebug(std::make_unique<DebugUtility>())
    , mPause(std::make_unique<Pause>())
    , mAssetsRenderTextureManager(std::make_unique<AssetsRenderTextureManager>())
    , mSceneMeshOperator(std::make_unique<SceneMeshOperator>())
    , mMode(DebugMode::LOG)
{
}

EngineFunctionManager::~EngineFunctionManager() = default;

void EngineFunctionManager::loadProperties(const rapidjson::Value& inObj) {
    mPause->loadProperties(inObj);
    mAssetsRenderTextureManager->loadProperties(inObj);
}

void EngineFunctionManager::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) {
    mPause->saveProperties(alloc, inObj);
    mAssetsRenderTextureManager->saveProperties(alloc, inObj);
}

void EngineFunctionManager::initialize(const IGameObjectsGetter* getter) {
    mDebug->initialize(getter, mPause.get());
    mPause->initialize();
    mAssetsRenderTextureManager->initialize();
}

void EngineFunctionManager::afterInitialize(const std::shared_ptr<Camera>& camera, const IMeshesGetter* getter) {
    mAssetsRenderTextureManager->afterInitialize(camera, getter);
}

void EngineFunctionManager::preUpdateProcess() {
    mDebug->preUpdateProcess();
}

void EngineFunctionManager::update() {
    mDebug->update();
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
    mDebug->draw(mMode, renderer, proj);
}

void EngineFunctionManager::draw3D(const Renderer& renderer, const Matrix4& viewProj) const {
    mAssetsRenderTextureManager->drawMeshes();
    mDebug->draw3D(renderer, viewProj);
}

DebugUtility& EngineFunctionManager::debug() const {
    return *mDebug;
}

IPause& EngineFunctionManager::pause() const {
    return *mPause;
}

AssetsRenderTextureManager& EngineFunctionManager::getAssetsRenderTextureManager() const {
    return *mAssetsRenderTextureManager;
}
