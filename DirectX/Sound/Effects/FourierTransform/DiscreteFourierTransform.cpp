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
    mComp.resize(N);
    mOutComp.resize(N);

    //窓関数
    hanningWindow(mWindowFunc.data(), N);

    auto sine = SineWaveGenerator::generate(441.f, 44100.f, 1.f, N);
    for (int i = 0; i < N; i++) {
        mComp[i].real(sine[i] * mWindowFunc[i]);
        mComp[i].imag(0.f);
    }
    //fft(mOutComp.data(), mComp.data(), N);
    for (int k = 0; k < N; k++) {
        for (int n = 0; n < N; n++) {
            auto temp = Math::TwoPI * k * n / N;
            float real = cosf(temp);
            float imag = -sinf(temp);
            std::complex<float> add(
                real * mComp[n].real() - imag * mComp[n].imag(),
                real * mComp[n].imag() + imag * mComp[n].real()
            );
            mOutComp[k] += add;
        }
    }

    WaveformOutput::outputComplexes("sine.csv", mOutComp.data(), mOutComp.size() / 2 - 1);

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
        mComp[i].real(inBuf[i] * mWindowFunc[i]);
        mComp[i].imag(0.f);
    }

    //高速フーリエ変換
    //fft(mOutComp.data(), mComp.data(), N);
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

void DiscreteFourierTransform::fft(std::complex<float>* out, const std::complex<float>* in, int N) {
    //fftの段数を計算
    const int NUMBER_OF_STAGE = log2(N);

    //fft
    for (int stage = 1; stage <= NUMBER_OF_STAGE; stage++) {
        for (int i = 0; i < pow2(stage - 1); i++) {
            for (int j = 0; j < pow2(NUMBER_OF_STAGE - stage); j++) {
                int n = pow2(NUMBER_OF_STAGE - stage + 1) * i + j;
                int m = pow2(NUMBER_OF_STAGE - stage) + n;
                int r = pow2(stage - 1) * j;
                float aReal = in[n].real();
                float aImag = in[n].imag();
                float bReal = in[m].real();
                float bImag = in[m].imag();
                float cReal = cosf(Math::TwoPI * r / N);
                float cImag = -sinf(Math::TwoPI * r / N);

                out[n].real(aReal + bReal);
                out[n].imag(aImag + bImag);
                if (stage < NUMBER_OF_STAGE) {
                    out[m].real((aReal - bReal) * cReal - (aImag - bImag) * cImag);
                    out[m].imag((aImag - bImag) * cReal + (aReal - bReal) * cImag);
                } else {
                    out[m].real(aReal - bReal);
                    out[m].imag(aImag - bImag);
                }
            }
        }
    }

    //インデックスの並び替えのためのテーブルの作成
    std::vector<int> index(N);
    for (int stage = 1; stage <= NUMBER_OF_STAGE; stage++) {
        for (int i = 0; i < pow2(stage - 1); i++) {
            index[pow2(stage - 1) + i] = index[i] + pow2(NUMBER_OF_STAGE - stage);
        }
    }

    //インデックスの並び替え
    for (int i = 0; i < N; i++) {
        if (index[i] > i) {
            float real = out[index[i]].real();
            float imag = out[index[i]].imag();
            out[index[i]].real(out[i].real());
            out[index[i]].imag(out[i].imag());
            out[i].real(real);
            out[i].imag(imag);
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
