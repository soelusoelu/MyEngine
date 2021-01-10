#include "EngineFunctionManager.h"
#include "EngineFuctionChanger.h"
#include "AssetsRenderer/AssetsRenderTextureManager.h"
#include "DebugManager/DebugManager.h"
#include "DebugManager/DebugLayer/DebugLayer.h"
#include "ModelViewer/ModelViewer.h"
#include "Pause/Pause.h"
#include "SceneMesh/SceneMeshOperator.h"
#include "../Component/Engine/Camera/Camera.h"
#include "../Component/Engine/Light/DirectionalLight.h"
#include "../Device/Renderer.h"

EngineFunctionManager::EngineFunctionManager()
    : mDebugManager(std::make_unique<DebugManager>())
    , mPause(std::make_unique<Pause>())
    , mFunctionChanger(std::make_unique<EngineFuctionChanger>())
    , mAssetsRenderTextureManager(std::make_unique<AssetsRenderTextureManager>())
    , mSceneMeshOperator(std::make_unique<SceneMeshOperator>())
    , mModelViewer(std::make_unique<ModelViewer>())
{
}

EngineFunctionManager::~EngineFunctionManager() = default;

void EngineFunctionManager::loadProperties(const rapidjson::Value& inObj) {
    mDebugManager->loadProperties(inObj);
    mPause->loadProperties(inObj);
    mFunctionChanger->loadProperties(inObj);
    mAssetsRenderTextureManager->loadProperties(inObj);
}

void EngineFunctionManager::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) {
    mDebugManager->saveProperties(alloc, inObj);
    mPause->saveProperties(alloc, inObj);
    mFunctionChanger->saveProperties(alloc, inObj);
    mAssetsRenderTextureManager->saveProperties(alloc, inObj);
}

void EngineFunctionManager::initialize(
    const std::shared_ptr<Camera>& camera,
    IEngineModeChanger* modeChanger,
    const IGameObjectsGetter* gameObjctsGetter,
    const IMeshesGetter* meshesGetter,
    const IFpsGetter* fpsGetter
) {
    mDebugManager->initialize(gameObjctsGetter, fpsGetter, mPause.get());
    mPause->initialize();
    mFunctionChanger->initialize(modeChanger);
    mAssetsRenderTextureManager->initialize(camera, mDebugManager->getDebugLayer().inspector(), meshesGetter);
    mSceneMeshOperator->initialize(camera, meshesGetter);
    mModelViewer->initialize(mAssetsRenderTextureManager.get(), mAssetsRenderTextureManager->getCallbackSelectAssetsTexture());
}

void EngineFunctionManager::preUpdateProcess() {
    mDebugManager->preUpdateProcess();
}

void EngineFunctionManager::update(EngineMode mode) {
    mAssetsRenderTextureManager->update(mode);
    mModelViewer->update(mode);
    mDebugManager->update();
    mPause->update();
    mFunctionChanger->update();
    mSceneMeshOperator->update();
}

void EngineFunctionManager::draw(EngineMode mode, const Renderer& renderer, Matrix4& proj) const {
    //レンダリング領域をデバッグに変更
    renderer.renderToDebug(proj);

    mAssetsRenderTextureManager->drawTextures(mode, proj);
    mPause->drawButton(proj);
    mFunctionChanger->draw(proj);
    mDebugManager->draw(mode, renderer, proj);
}

void EngineFunctionManager::draw3D(
    EngineMode mode,
    const Renderer& renderer,
    const Camera& camera,
    const DirectionalLight& dirLight
) const {
    mAssetsRenderTextureManager->drawMeshes(mode);
    mModelViewer->draw(mode, dirLight.getDirection(), dirLight.getLightColor());
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
