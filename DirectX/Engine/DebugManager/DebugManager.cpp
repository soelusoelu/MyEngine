#include "DebugManager.h"
#include "DebugLayer/DebugLayer.h"
#include "DebugUtility/DebugUtility.h"
#include "../../Device/DrawString.h"

DebugManager::DebugManager()
    : FileOperator("DebugManager")
    , mStringDrawer(std::make_unique<DrawString>())
    , mDebugLayer(std::make_unique<DebugLayer>())
{
}

DebugManager::~DebugManager() {
    DebugUtility::instance().finalize();
}

void DebugManager::initialize(
    const IGameObjectsGetter* gameObjectsGetter,
    const IFpsGetter* fpsGetter,
    const IPause* pause
) {
    mStringDrawer->initialize();
    mDebugLayer->initialize(gameObjectsGetter, fpsGetter);
    DebugUtility::instance().initialize(pause);
}

void DebugManager::preUpdateProcess() {
    //描画済み文字列を削除する
    mStringDrawer->clear();
    DebugUtility::instance().preUpdateProcess();
}

void DebugManager::update() {
    mDebugLayer->update();
}

void DebugManager::draw2D(const Renderer& renderer, Matrix4& proj) const {
    DebugUtility::instance().drawLine2D(renderer, proj);
}

void DebugManager::drawDebug2D(EngineMode mode, Matrix4& proj) const {
    mDebugLayer->draw(mode, *mStringDrawer, proj);
    DebugUtility::instance().draw(mode, *mStringDrawer);
    mStringDrawer->drawAll(proj);
}

void DebugManager::draw3D(
    EngineMode mode,
    const Renderer& renderer,
    const Matrix4& viewProj
) const {
    DebugUtility::instance().draw3D(mode, renderer, viewProj);
}

DebugLayer& DebugManager::getDebugLayer() const {
    return *mDebugLayer;
}

void DebugManager::childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    if (mode == FileMode::LOAD) {
        mStringDrawer->writeAndRead(inObj, alloc, mode);
    }
    mDebugLayer->writeAndRead(inObj, alloc, mode);
    DebugUtility::instance().writeAndRead(inObj, alloc, mode);
}
