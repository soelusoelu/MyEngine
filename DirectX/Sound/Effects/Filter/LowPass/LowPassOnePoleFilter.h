﻿#pragma once

#include "../../SoundEffectUtility.h"
#include <xapobase.h>

#pragma comment(lib, "xapobase.lib")

namespace MyFilter {
class __declspec(uuid("{835656D0-10B9-4A70-B740-80486D4383A5}"))
    LowPassOnePoleFilter : public CXAPOParametersBase {
public:
    LowPassOnePoleFilter();
    ~LowPassOnePoleFilter();

    //最初の一回だけ呼ばれる、値を保持するための関数
    STDMETHOD(LockForProcess)(
        UINT32 InputLockedParameterCount,
        const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters,
        UINT32 OutputLockedParameterCount,
        const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters
        );

    //毎秒100回処理されるメイン関数
    //実際にエフェクトを掛けるとこ
    STDMETHOD_(void, Process)(
        UINT32 InputProcessParameterCount,
        const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters,
        UINT32 OutputProcessParameterCount,
        XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters,
        BOOL IsEnabled
        );

    //もしアクセスしたとしても落とさないために
    STDMETHOD_(void, SetParameters)(
        const void* pParameters,
        UINT32 ParameterByteSize
        );

private:
    LowPassOnePoleFilter(const LowPassOnePoleFilter&) = delete;
    LowPassOnePoleFilter& operator=(const LowPassOnePoleFilter&) = delete;

    void lowPassOnePoleFilter(const XAPO_PROCESS_BUFFER_PARAMETERS& inParam, XAPO_PROCESS_BUFFER_PARAMETERS& outParam);

private:
    //プロパティ
    static inline XAPO_REGISTRATION_PROPERTIES xapoRegProp_ = {
        __uuidof(LowPassOnePoleFilter),
        L"LowPassOnePoleFilter",
        L"Copyright",
        1, 0,
        XAPOBASE_DEFAULT_FLAG,
        1, 1, 1, 1
    };

    //入力フォーマットの記憶場所
    WAVEFORMATEX mInputFmt;
    //出力フォーマットの記憶場所
    WAVEFORMATEX mOutputFmt;
    //パラメータの受信場所
    //カットオフ周波数
    float mFrequency[EFFECT_PARAMETER_SIZE];

    //最後の演算ボリューム値
    float mLastVolume;
};
}
