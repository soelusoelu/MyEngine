#include "DiscreteFourierTransform.h"
#include "../../Factory/SineWaveGenerator.h"
#include "../../File/WaveformOutput.h"
#include "../../../DebugLayer/Debug.h"
#include "../../../Input/Input.h"
#include "../../../Math/Math.h"

DiscreteFourierTransform::DiscreteFourierTransform() :
    CXAPOParametersBase(&xapoRegProp_, reinterpret_cast<BYTE*>(mParam), sizeof(float), FALSE),
    mInputFmt(),
    mOutputFmt() {
}

DiscreteFourierTransform::~DiscreteFourierTransform() = default;

STDMETHODIMP_(HRESULT __stdcall) DiscreteFourierTransform::LockForProcess(UINT32 InputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters, UINT32 OutputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters) {
    mInputFmt = *pInputLockedParameters[0].pFormat;
    mOutputFmt = *pOutputLockedParameters[0].pFormat;

    constexpr int N = 512;
    mWindowFunc.resize(N);
    mReal.resize(N);
    mImaginary.resize(N, 0.f);
    mOutReal.resize(N);
    mOutImaginary.resize(N);

    //窓関数
    hanningWindow(mWindowFunc.data(), N);

    auto sine = SineWaveGenerator::generate(441.f, 44100.f, 1.f, N);
    //for (int i = 0; i < N; i++) {
    //    sine[i] = sine[i] * mWindowFunc[i];
    //}
    //fft(mOutReal.data(), mOutImaginary.data(), sine.data(), mImaginary.data(), N);
    //float real, imag;
    //for (int k = 0; k < N; k++) {
    //    for (int n = 0; n < N; n++) {
    //        real = cosf(Math::TwoPI * k * n / N);
    //        imag = -sinf(Math::TwoPI * k * n / N);
    //        mOutReal[k] += real * sine[n] - imag * mImaginary[n];
    //        mOutImaginary[k] += real * mImaginary[n] + imag * sine[n];
    //    }
    //}

    //WaveformOutput::outputWaveform("sineReal.csv", mOutReal.data(), sine.size() / 2 - 1);
    //WaveformOutput::outputWaveform("sineImag.csv", mOutImaginary.data(), sine.size() / 2 - 1);

    return CXAPOParametersBase::LockForProcess(InputLockedParameterCount, pInputLockedParameters, OutputLockedParameterCount, pOutputLockedParameters);
}

STDMETHODIMP_(void __stdcall) DiscreteFourierTransform::Process(UINT32 InputProcessParameterCount, const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters, UINT32 OutputProcessParameterCount, XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters, BOOL IsEnabled) {
    const auto& inParam = pInputProcessParameters[0];
    auto& outParam = pOutputProcessParameters[0];

    if (IsEnabled) { //有効
        discreteFourierTransform(inParam, outParam);
        memcpy(outParam.pBuffer, inParam.pBuffer, mOutputFmt.nBlockAlign * inParam.ValidFrameCount);
    } else { //無効
        if (inParam.pBuffer != outParam.pBuffer) {
            memcpy(outParam.pBuffer, inParam.pBuffer, mOutputFmt.nBlockAlign * inParam.ValidFrameCount);
        }
    }
}

STDMETHODIMP_(void __stdcall) DiscreteFourierTransform::SetParameters(const void* pParameters, UINT32 ParameterByteSize) {
    if (ParameterByteSize == sizeof(float)) {
        CXAPOParametersBase::SetParameters(pParameters, ParameterByteSize);
    } else {
        Debug::logWarning("Wrong size of parameter");
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

    //fftで計算するサンプル数
    const int N = mWindowFunc.size();

    //波形に窓関数を掛ける
    for (int i = 0; i < N; i++) {
        mReal[i] = inBuf[i] * mWindowFunc[i];
    }

    //高速フーリエ変換
    fft(mOutReal.data(), mOutImaginary.data(), mReal.data(), mImaginary.data(), N);
}

void DiscreteFourierTransform::hanningWindow(float* out, int N) {
    if (N % 2 == 0) {
        for (int n = 0; n < N; n++) {
            out[n] = 0.5f - 0.5f * cosf(Math::TwoPI * n / N);
        }
    } else {
        for (int n = 0; n < N; n++) {
            out[n] = 0.5f - 0.5f * cosf(Math::TwoPI * (n + 0.5f) / N);
        }
    }
}

void DiscreteFourierTransform::fft(float* outReal, float* outImag, const float* xReal, const float* xImag, int N) {
    //fftの段数を計算
    const int numberOfStage = log2(N);

    //fft
    for (int stage = 1; stage <= numberOfStage; stage++) {
        for (int i = 0; i < pow2(stage - 1); i++) {
            for (int j = 0; j < pow2(numberOfStage - stage); j++) {
                int n = pow2(numberOfStage - stage + 1) * i + j;
                int m = pow2(numberOfStage - stage) + n;
                int r = pow2(stage - 1) * j;
                float aReal = xReal[n];
                float aImag = xImag[n];
                float bReal = xReal[m];
                float bImag = xImag[m];
                float cReal = cosf(Math::TwoPI * r / N);
                float cImag = -sinf(Math::TwoPI * r / N);

                outReal[n] = aReal + bReal;
                outImag[n] = aImag + bImag;
                if (stage < numberOfStage) {
                    outReal[m] = (aReal - bReal) * cReal - (aImag - bImag) * cImag;
                    outImag[m] = (aImag - bImag) * cReal + (aReal - bReal) * cImag;
                } else {
                    outReal[m] = aReal - bReal;
                    outImag[m] = aImag - bImag;
                }
            }
        }
    }

    //インデックスの並び替えのためのテーブルの作成
    std::vector<int> index(N);
    for (int stage = 1; stage <= numberOfStage; stage++) {
        for (int i = 0; i < pow2(stage - 1); i++) {
            index[pow2(stage - 1) + i] = index[i] + pow2(numberOfStage - stage);
        }
    }

    //インデックスの並び替え
    float real, imag;
    for (int i = 0; i < N; i++) {
        if (index[i] > i) {
            real = outReal[index[i]];
            imag = outImag[index[i]];
            outReal[index[i]] = outReal[i];
            outImag[index[i]] = outImag[i];
            outReal[i] = real;
            outImag[i] = imag;
        }
    }
}

int DiscreteFourierTransform::log2(int x) const {
    int y = 0;
    while (x > 1) {
        x >>= 1;
        y++;
    }

    return y;
}

int DiscreteFourierTransform::pow2(int x) const {
    int y = 0;
    if (x == 0) {
        y = 1;
    } else {
        y = 2 << (x - 1);
    }

    return y;
}
