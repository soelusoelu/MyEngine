#include "DebugUtility.h"
#include "FixedDebugInformation.h"
#include "Hierarchy.h"
#include "ImGuiInspector.h"
#include "Inspector.h"
#include "Log.h"
#include "Pause.h"
#include "PointRenderer.h"
#include "AssetsRenderer/AssetsRenderTextureManager.h"
#include "LineRenderer/LineRenderer2D.h"
#include "LineRenderer/LineRenderer3D.h"
#include "../Device/DrawString.h"
#include "../System/GlobalFunction.h"

DebugUtility::DebugUtility()
    : mDrawString(std::make_unique<DrawString>())
    , mLog(std::make_unique<Log>())
    , mFixedDebugInfo(std::make_unique<FixedDebugInformation>())
    , mHierarchy(std::make_unique<Hierarchy>())
    , mInspector(std::make_unique<ImGuiInspector>())
    , mPause(std::make_unique<Pause>())
    , mPointRenderer(std::make_unique<PointRenderer>())
    , mLineRenderer2D(std::make_unique<LineRenderer2D>())
    , mLineRenderer3D(std::make_unique<LineRenderer3D>())
    , mAssetsRenderTextureManager(std::make_unique<AssetsRenderTextureManager>())
    , mMode(DebugMode::LOG)
{
}

DebugUtility::~DebugUtility() = default;

DebugUtility& DebugUtility::instance() {
    if (!mInstance) {
        mInstance = new DebugUtility();
    }
    return *mInstance;
}

void DebugUtility::loadProperties(const rapidjson::Value& inObj) {
    mDrawString->loadProperties(inObj);
    mLog->loadProperties(inObj);
    mFixedDebugInfo->loadProperties(inObj);
    mHierarchy->loadProperties(inObj);
    mInspector->loadProperties(inObj);
    mPause->loadProperties(inObj);
    mAssetsRenderTextureManager->loadProperties(inObj);
}

void DebugUtility::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) {
    mLog->saveProperties(alloc, inObj);
    mFixedDebugInfo->saveProperties(alloc, inObj);
    mHierarchy->saveProperties(alloc, inObj);
    mInspector->saveProperties(alloc, inObj);
    mPause->saveProperties(alloc, inObj);
    mAssetsRenderTextureManager->saveProperties(alloc, inObj);
}

void DebugUtility::initialize() {
    mDrawString->initialize();
    mLog->initialize();
    mFixedDebugInfo->initialize();
    mHierarchy->initialize();
    mPause->initialize();
    mPointRenderer->initialize();
    mLineRenderer2D->initialize();
    mLineRenderer3D->initialize();
    mAssetsRenderTextureManager->initialize();
}

void DebugUtility::finalize() {
    safeDelete(mInstance);
}

void DebugUtility::update() {
    mHierarchy->update();
    mPause->update();
    mAssetsRenderTextureManager->update();
}

void DebugUtility::draw(const Matrix4& proj) const {
    if (mMode == DebugMode::LOG) {
        mLog->drawLogs(*mDrawString);
        mFixedDebugInfo->draw(*mDrawString);
    } else if (mMode == DebugMode::MESH_TEXTURES) {
        mAssetsRenderTextureManager->drawTextures(proj);
    }

    mHierarchy->drawGameObjects(*mDrawString);
    mInspector->drawInspect();
    mPause->drawButton(proj);
    mDrawString->drawAll(proj);
}

void DebugUtility::draw3D() const {
    mAssetsRenderTextureManager->drawMeshes();
}

void DebugUtility::changeScene(const std::string& scene) {
    if (scene == DebugSceneName::MAP_EDITOR) {
        mMode = DebugMode::MESH_TEXTURES;
    } else {
        mMode = DebugMode::LOG;
    }
}

void DebugUtility::drawStringClear() {
    mDrawString->clear();
}

Log& DebugUtility::log() const {
    return *mLog;
}

FixedDebugInformation& DebugUtility::fixedDebugInfo() const {
    return *mFixedDebugInfo;
}

Hierarchy& DebugUtility::hierarchy() const {
    return *mHierarchy;
}

//Inspector& DebugUtility::inspector() {
//    return *mInspector;
//}

ImGuiInspector& DebugUtility::inspector() const {
    return *mInspector;
}

Pause& DebugUtility::pause() const {
    return *mPause;
}

PointRenderer& DebugUtility::pointRenderer() const {
    return *mPointRenderer;
}

LineRenderer2D& DebugUtility::lineRenderer2D() const {
    return *mLineRenderer2D;
}

LineRenderer3D& DebugUtility::lineRenderer3D() const {
    return *mLineRenderer3D;
}

AssetsRenderTextureManager& DebugUtility::assetsRenderTextureManager() const {
    return *mAssetsRenderTextureManager;
}
