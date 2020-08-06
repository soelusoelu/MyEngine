#pragma once

#include "SoundBuffer.h"

//バッファ追加インターフェース
class IBufferSubmitter {
public:
    virtual ~IBufferSubmitter() = default;
    virtual void submitSourceBuffer(const SoundBuffer& buffer) const = 0;
};
