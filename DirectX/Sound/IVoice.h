#pragma once

#include <xaudio2.h>

//ボイスインターフェース
class IVoice {
public:
    virtual ~IVoice() = default;
    virtual IXAudio2Voice* getXAudio2Voice() const = 0;
};
