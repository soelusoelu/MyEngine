#pragma once

#include "../../Component.h"

class OctopusFootCommonSetting
    : public Component
{
    friend class OctopusFoot;

public:
    OctopusFootCommonSetting();
    ~OctopusFootCommonSetting();
    OctopusFootCommonSetting(const OctopusFootCommonSetting&) = delete;
    OctopusFootCommonSetting& operator=(const OctopusFootCommonSetting&) = delete;

    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

private:
    int mHp;
};
