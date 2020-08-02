#pragma once

#include <xaudio2.h>

//エフェクトパラメータインターフェース
class IEffectParameter {
public:
    virtual ~IEffectParameter() = default;
    virtual void setEffectParameters(int effectID, const void* parameters, unsigned parametersByteSize, unsigned operationSet = XAUDIO2_COMMIT_NOW) = 0;
    virtual void getEffectParameters(int effectID, void* parameters, unsigned parametersByteSize) const = 0;
};
