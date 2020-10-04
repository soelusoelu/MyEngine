#pragma once

#include "../SoundEffectUtility.h"
#include <xapobase.h>
#include <complex>
#include <vector>

#pragma comment(lib, "xapobase.lib")

//離散フーリエ変換
class __declspec(uuid("{179D2876-1019-4EE0-8BCB-83EF5E8CB1AA}"))
    DiscreteFourierTransform : public CXAPOParametersBase {
public:
    DiscreteFourierTransform();
    ~DiscreteFourierTransform();

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

    //パラメータ設定関数
    STDMETHOD_(void, SetParameters)(
        const void* pParameters,
        UINT32 ParameterByteSize
        );

private:
    DiscreteFourierTransform(const DiscreteFourierTransform&) = delete;
    DiscreteFourierTransform& operator=(const DiscreteFourierTransform&) = delete;

    void discreteFourierTransform(const XAPO_PROCESS_BUFFER_PARAMETERS& inParam, XAPO_PROCESS_BUFFER_PARAMETERS& outParam);
    //窓関数(ハニング窓)
    void hanningWindow(float* out, int N);
    //高速フーリエ変換
    void fft(std::complex<float>* out, const std::complex<float>* in, int N);
    int log2(int x) const;
    int pow2(int x) const;

private:
    //プロパティ
    static inline XAPO_REGISTRATION_PROPERTIES xapoRegProp_ = {
        __uuidof(DiscreteFourierTransform),
        L"DiscreteFourierTransform",
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
    float mParam[EFFECT_PARAMETER_SIZE];

    std::vector<float> mWindowFunc;
    std::vector<std::complex<float>> mComp;
    std::vector<std::complex<float>> mOutComp;
};
