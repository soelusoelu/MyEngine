#pragma once

#include <xaudio2.h>
#include <string>

//サウンド読み込み用インターフェース
class ISoundLoader {
public:
    virtual ~ISoundLoader() = default;
    virtual bool loadFromFile(WAVEFORMATEX* format, const std::string& fileName) = 0;
};
