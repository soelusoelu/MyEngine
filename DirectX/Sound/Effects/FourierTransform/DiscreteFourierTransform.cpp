#include "DiscreteFourierTransform.h"
#include "FourierTransform.h"
#include "WindowFunction.h"
#include "../../Factory/SineWaveGenerator.h"
#include "../../File/WaveformOutput.h"
#include "../../Volume/SoundVolume.h"
#include "../../../DebugLayer/Debug.h"
#include "../../../Input/Input.h"
#include "../../../Math/Math.h"
#include <algorithm>

DiscreteFourierTransform::DiscreteFourierTransform() :
    CXAPOParametersBase(&xapoRegProp_, reinterpret_cast<BYTE*>(mParam), sizeof(ComplexArray), TRUE),
    mFourier(std::make_unique<FourierTransform>()),
    mInputFmt(),
    mOutputFmt() {
}

DiscreteFourierTransform::~DiscreteFourierTransform() = default;

STDMETHODIMP_(HRESULT __stdcall) DiscreteFourierTransform::LockForProcess(UINT32 InputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters, UINT32 OutputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters) {
    mInputFmt = *pInputLockedParameters[0].pFormat;
    mOutputFmt = *pOutputLockedParameters[0].pFormat;

    constexpr int N = 512;
    mFourier->initialize(N);
    mComp.resize(N);
    mOutComp.resize(N);

    //auto sine = SineWaveGenerator::generate(441.f, 44100.f, 1.f, N);
    //WindowFunction::hanning(mComp.data(), sine.data(), mComp.size());
    //mFourier->fastFourierTransform(mOutComp.data(), mComp.data(), N);

    //for (size_t i = 0; i < N / 2 - 1; i++) {
    //    auto vo = mOutComp[i].imag();
    //    mOutComp[i].imag(SoundVolume::amplitudeRatioToDecibels(vo));
    //}

    //WaveformOutput::outputComplexes("out.csv", mOutComp.data(), mOutComp.size() / 2 - 1);

    return CXAPOParametersBase::LockForProcess(InputLockedParameterCount, pInputLockedParameters, OutputLockedParameterCount, pOutputLockedParameters);
}

STDMETHODIMP_(void __stdcall) DiscreteFourierTransform::Process(UINT32 InputProcessParameterCount, const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters, UINT32 OutputProcessParameterCount, XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters, BOOL IsEnabled) {
    const auto& inParam = pInputProcessParameters[0];
    auto& outParam = pOutputProcessParameters[0];

    if (IsEnabled) {
        //高速フーリエ変換
        discreteFourierTransform(inParam, outParam);

        auto param = reinterpret_cast<ComplexArray*>(BeginProcess());

        const int N = mOutComp.size();
        if (param->size() != N) {
            param->resize(N);
        }
        std::copy(mOutComp.begin(), mOutComp.end(), param->begin());

        EndProcess();
    }

    //波形はそのまま
    if (inParam.pBuffer != outParam.pBuffer) {
        memcpy(outParam.pBuffer, inParam.pBuffer, mOutputFmt.nBlockAlign * inParam.ValidFrameCount);
    }
}

void DiscreteFourierTransform::discreteFourierTransform(const XAPO_PROCESS_BUFFER_PARAMETERS& inParam, XAPO_PROCESS_BUFFER_PARAMETERS& outParam) {
    //サイレントなら計算はしない
    if (inParam.BufferFlags == XAPO_BUFFER_FLAGS::XAPO_BUFFER_SILENT) {
        outParam.BufferFlags = inParam.BufferFlags;
        return;
    }

    //バッファの取得
    float* inBuf = static_cast<float*>(inParam.pBuffer);

    //サンプル数
    const int N = mComp.size();

    //波形に窓関数を掛ける
    WindowFunction::hanning(mComp.data(), inBuf, N);

    //高速フーリエ変換
    mFourier->fastFourierTransform(mOutComp.data(), mComp.data(), N);

    //デシベル値に変換
    for (size_t i = 0; i < N / 2 - 1; i++) {
        mOutComp[i].imag(SoundVolume::amplitudeRatioToDecibels(mOutComp[i].imag()));
    }
}
