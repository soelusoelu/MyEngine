#pragma once

#include "../../Component.h"

class CrossHair
    : public Component
{
public:
    CrossHair();
    ~CrossHair();
    CrossHair(const CrossHair&) = delete;
    CrossHair& operator=(const CrossHair&) = delete;

    virtual void update() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

private:
    float mOffset;
    float mLength;
};
