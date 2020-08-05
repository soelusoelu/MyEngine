#pragma once

#include "WaveformData.h"
#include <memory>
#include <string>

//サウンド読み込み用インターフェース
class ISoundLoader {
public:
    virtual ~ISoundLoader() = default;
    virtual void loadFromFile(std::shared_ptr<WaveformData>* data, const std::string& fileName) const = 0;
};
