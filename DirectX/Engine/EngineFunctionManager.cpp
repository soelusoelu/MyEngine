#include "EngineFunctionManager.h"
#include "AssetsRenderer/AssetsRenderTextureManager.h"
#include "DebugManager/DebugManager.h"
#include "DebugManager/DebugLayer/DebugLayer.h"
#include "ModelViewer/ModelViewer.h"
#include "Pause/Pause.h"
#include "SceneMesh/SceneMeshOperator.h"
#include "../Component/Engine/Camera/Camera.h"
#include "../Device/Renderer.h"

EngineFunctionManager::EngineFunctionManager()
    : mDebugManager(std::make_unique<DebugManager>())
    , mPause(std::make_unique<Pause>())
    , mAssetsRenderTextureManager(std::make_unique<AssetsRenderTextureManager>())
    , mSceneMeshOperator(std::make_unique<SceneMeshOperator>())
    , mModelViewer(std::make_unique<ModelViewer>())
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

void EngineFunctionManager::initialize(
    const std::shared_ptr<Camera>& camera,
    const IGameObjectsGetter* gameObjctsGetter,
    const IMeshesGetter* meshesGetter,
    const IFpsGetter* fpsGetter
) {
    mDebugManager->initialize(gameObjctsGetter, fpsGetter, mPause.get());
    mPause->initialize();
    mAssetsRenderTextureManager->initialize(camera, mDebugManager->getDebugLayer().inspector(), meshesGetter);
    mSceneMeshOperator->initialize(camera, meshesGetter);
    mModelViewer->initialize();
}

void EngineFunctionManager::preUpdateProcess() {
    mDebugManager->preUpdateProcess();
}

void EngineFunctionManager::update(EngineMode mode) {
    if (mode == EngineMode::MAP_EDITOR) {
        mAssetsRenderTextureManager->update();
    } else if (mode == EngineMode::MODEL_VIEWER) {

    }

    mDebugManager->update();
    mPause->update();
    mSceneMeshOperator->update();
}

void EngineFunctionManager::draw(EngineMode mode, const Renderer& renderer, Matrix4& proj) const {
    //レンダリング領域をデバッグに変更
    renderer.renderToDebug(proj);

    if (mode == EngineMode::GAME) {

    } else if (mode == EngineMode::MAP_EDITOR) {
        mAssetsRenderTextureManager->drawTextures(proj);
    }

    mPause->drawButton(proj);
    mDebugManager->draw(mode, renderer, proj);
}

void EngineFunctionManager::draw3D(
    EngineMode mode,
    const Renderer& renderer,
    const Camera& camera,
    const DirectionalLight& dirLight
) const {
    if (mode == EngineMode::MAP_EDITOR) {
        mAssetsRenderTextureManager->drawMeshes();
    } else if (mode == EngineMode::MODEL_VIEWER) {
        mModelViewer->draw(camera, dirLight);
    }

    mDebugManager->draw3D(mode, renderer, camera.getViewProjection());
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
