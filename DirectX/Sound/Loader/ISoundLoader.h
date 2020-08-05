#pragma once

#include "WaveformData.h"
#include <memory>
#include <string>

//サウンド読み込み用インターフェース
class ISoundLoader {
public:
    virtual ~ISoundLoader() = default;
    virtual std::shared_ptr<WaveformData> loadFromFile(const std::string& fileName) = 0;
};
