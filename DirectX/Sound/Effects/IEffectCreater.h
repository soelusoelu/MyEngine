#pragma once

#include <xaudio2.h>

//エフェクト作成インターフェース
class IEffectCreater {
public:
    virtual ~IEffectCreater() = default;
    virtual int createEffect(IUnknown* target, bool isApply = true) = 0;
};
