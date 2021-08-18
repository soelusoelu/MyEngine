#include "OctopusFootCommonSetting.h"
#include "../../../Utility/JsonHelper.h"

OctopusFootCommonSetting::OctopusFootCommonSetting()
    : Component()
    , mHp(0)
{
}

OctopusFootCommonSetting::~OctopusFootCommonSetting() = default;

void OctopusFootCommonSetting::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mHp, "HP", inObj, alloc, mode);
}
