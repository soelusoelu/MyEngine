#include "DebugUtility.h"
#include "Log.h"
#include "PointRenderer.h"
#include "LineRenderer/LineInstancingDrawer.h"
#include "LineRenderer/LineRenderer2D.h"
#include "../../../Device/DrawString.h"
#include "../../../Device/Renderer.h"
#include "../../../System/GlobalFunction.h"

DebugUtility::DebugUtility()
    : FileOperator("DebugUtility")
    , mPause(nullptr)
    , mLog(std::make_unique<Log>())
    , mPointRenderer(std::make_unique<PointRenderer>())
    , mLineRenderer2D(std::make_unique<LineRenderer2D>())
    , mLineRenderer3D(std::make_unique<LineInstancingDrawer>())
{
}

DebugUtility::~DebugUtility() = default;

DebugUtility& DebugUtility::instance() {
    if (!mInstance) {
        mInstance = new DebugUtility();
    }
    return *mInstance;
}

void DebugUtility::initialize(const IPause* pause) {
    mPause = pause;
    mLog->initialize();
    mPointRenderer->initialize();
    mLineRenderer2D->initialize();
    mLineRenderer3D->initialize();
}

void DebugUtility::finalize() {
    safeDelete(mInstance);
}

void DebugUtility::preUpdateProcess() {
    //ポーズ中なら終了
    if (mPause->isPausing()) {
        return;
    }

    //描画情報を削除
    mPointRenderer->clear();
    mLineRenderer2D->clear();
    mLineRenderer3D->clear();
}

void DebugUtility::draw(EngineMode mode, DrawString& drawer) const {
    if (mode == EngineMode::GAME) {
        mLog->drawLogs(drawer);
    }
}

void DebugUtility::drawLine2D(const Renderer& renderer, Matrix4& proj) const {
    renderer.renderLine2D(proj);
    mLineRenderer2D->draw(proj);
}

void DebugUtility::draw3D(EngineMode mode, const Renderer& renderer, const Matrix4& viewProj) const {
    renderer.renderPointLine3D();
    renderer.renderPoint3D();
    mPointRenderer->draw(viewProj);
    renderer.renderLine3D();
    mLineRenderer3D->draw(viewProj);
}

Log& DebugUtility::log() const {
    return *mLog;
}

PointRenderer& DebugUtility::pointRenderer() const {
    return *mPointRenderer;
}

LineRenderer2D& DebugUtility::lineRenderer2D() const {
    return *mLineRenderer2D;
}

LineInstancingDrawer& DebugUtility::lineRenderer3D() const {
    return *mLineRenderer3D;
}

void DebugUtility::childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    mLog->writeAndRead(inObj, alloc, mode);
}
