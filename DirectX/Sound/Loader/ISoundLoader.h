#pragma once

#include <xaudio2.h>
#include <string>

//サウンド読み込み用インターフェース
class ISoundLoader {
public:
    virtual ~ISoundLoader() = default;
    virtual bool loadFromFile(WAVEFORMATEX* format, const std::string& fileName) = 0;
    virtual unsigned read(BYTE** buffer, unsigned size) = 0;
    virtual void seek(int offset) = 0;
    virtual unsigned size() const = 0;
};
