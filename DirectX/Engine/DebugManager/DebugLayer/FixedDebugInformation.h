#pragma once

#include "../../../Math/Math.h"
#include "../../../System/FpsCounter/IFpsGetter.h"
#include "../../../Device/FileOperator.h"

class DrawString;

class FixedDebugInformation
    : public FileOperator
{
public:
    FixedDebugInformation();
    ~FixedDebugInformation();
    void initialize(const IFpsGetter* getter);
    void draw(DrawString& drawString) const;

private:
    FixedDebugInformation(const FixedDebugInformation&) = delete;
    FixedDebugInformation& operator=(const FixedDebugInformation&) = delete;

    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

private:
    const IFpsGetter* mFpsGetter;
    //文字のスケール
    Vector2 mScale;
    //FPSを表示する位置 固定
    Vector2 mFPSPos;
};
