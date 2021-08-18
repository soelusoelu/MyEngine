#pragma once

#include "Inspector/IInspector.h"
#include "../../EngineMode.h"
#include "../../../Device/FileOperator.h"
#include "../../../GameObject/IGameObjectsGetter.h"
#include "../../../Math/Math.h"
#include "../../../System/FpsCounter/IFpsGetter.h"
#include <memory>
#include <string>

class DrawString;
class FixedDebugInformation;
class Hierarchy;
class ImGuiInspector;

class DebugLayer
    : public FileOperator
{
public:
    DebugLayer();
    ~DebugLayer();
    void initialize(const IGameObjectsGetter* gameObjectsGetter, const IFpsGetter* fpsGetter);
    void update();
    void draw(EngineMode mode, DrawString& drawer, Matrix4& proj) const;
    FixedDebugInformation& fixedDebugInfo() const;
    Hierarchy& hierarchy() const;
    IInspector* inspector() const;

private:
    DebugLayer(const DebugLayer&) = delete;
    DebugLayer& operator=(const DebugLayer&) = delete;

    virtual void childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

private:
    std::unique_ptr<FixedDebugInformation> mFixedDebugInfo;
    std::unique_ptr<Hierarchy> mHierarchy;
    std::unique_ptr<ImGuiInspector> mInspector;
};
