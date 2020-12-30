#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>

class DrawString;
class FixedDebugInformation;
class Hierarchy;
class Inspector;
class ImGuiInspector;
class Log;
class Pause;
class PointRenderer;
class LineRenderer2D;
class LineRenderer3D;
class AssetsRenderTextureManager;

class DebugUtility {
public:
    DebugUtility();
    ~DebugUtility();
    static DebugUtility& instance();
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj);
    void initialize();
    void finalize();
    void update();
    void draw(const Matrix4& proj) const;
    void draw3D() const;
    //DrawStringが保持する文字列をすべて削除
    void drawStringClear();
    Log& log() const;
    FixedDebugInformation& fixedDebugInfo() const;
    Hierarchy& hierarchy() const;
    //Inspector& inspector() const;
    ImGuiInspector& inspector() const;
    Pause& pause() const;
    PointRenderer& pointRenderer() const;
    LineRenderer2D& lineRenderer2D() const;
    LineRenderer3D& lineRenderer3D() const;
    AssetsRenderTextureManager& assetsRenderTextureManager() const;

private:
    DebugUtility(const DebugUtility&) = delete;
    DebugUtility& operator=(const DebugUtility&) = delete;

private:
    static inline DebugUtility* mInstance = nullptr;

    std::unique_ptr<DrawString> mDrawString;
    std::unique_ptr<Log> mLog;
    std::unique_ptr<FixedDebugInformation> mFixedDebugInfo;
    std::unique_ptr<Hierarchy> mHierarchy;
    //std::unique_ptr<Inspector> mInspector;
    std::unique_ptr<ImGuiInspector> mInspector;
    std::unique_ptr<Pause> mPause;
    std::unique_ptr<PointRenderer> mPointRenderer;
    std::unique_ptr<LineRenderer2D> mLineRenderer2D;
    std::unique_ptr<LineRenderer3D> mLineRenderer3D;
    std::unique_ptr<AssetsRenderTextureManager> mAssetsRenderTextureManager;
};
