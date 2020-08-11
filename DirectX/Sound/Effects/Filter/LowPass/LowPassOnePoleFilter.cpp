#include "LowPassOnePoleFilter.h"
#include "../../../../DebugLayer/Debug.h"
#include "../../../../Math/Math.h"

MyFilter::LowPassOnePoleFilter::LowPassOnePoleFilter() :
    CXAPOParametersBase(&xapoRegProp_, reinterpret_cast<BYTE*>(mFrequency), sizeof(float), FALSE),
    mInputFmt(),
    mOutputFmt(),
    mFrequency{ 1.f, 1.f, 1.f },
    mLastVolume(0.f) {
}

MyFilter::LowPassOnePoleFilter::~LowPassOnePoleFilter() = default;

STDMETHODIMP_(HRESULT __stdcall) MyFilter::LowPassOnePoleFilter::LockForProcess(UINT32 InputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters, UINT32 OutputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters) {
    mInputFmt = *pInputLockedParameters[0].pFormat;
    mOutputFmt = *pOutputLockedParameters[0].pFormat;

    return CXAPOParametersBase::LockForProcess(InputLockedParameterCount, pInputLockedParameters, OutputLockedParameterCount, pOutputLockedParameters);
}

STDMETHODIMP_(void __stdcall) MyFilter::LowPassOnePoleFilter::Process(UINT32 InputProcessParameterCount, const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters, UINT32 OutputProcessParameterCount, XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters, BOOL IsEnabled) {
    const auto& inParam = pInputProcessParameters[0];
    auto& outParam = pOutputProcessParameters[0];

    if (IsEnabled) { //有効
        lowPassOnePoleFilter(inParam, outParam);
    } else { //無効
        if (inParam.pBuffer != outParam.pBuffer) {
            memcpy(outParam.pBuffer, inParam.pBuffer, mOutputFmt.nBlockAlign * inParam.ValidFrameCount);
        }
    }
}

STDMETHODIMP_(void __stdcall) MyFilter::LowPassOnePoleFilter::SetParameters(const void* pParameters, UINT32 ParameterByteSize) {
    if (ParameterByteSize == sizeof(float)) {
        CXAPOParametersBase::SetParameters(pParameters, ParameterByteSize);
    } else {
        Debug::logWarning("Wrong size of parameter");
    }
}

void MyFilter::LowPassOnePoleFilter::lowPassOnePoleFilter(const XAPO_PROCESS_BUFFER_PARAMETERS& inParam, XAPO_PROCESS_BUFFER_PARAMETERS& outParam) {
    //サイレントなら計算はしない
    if (inParam.BufferFlags == XAPO_BUFFER_FLAGS::XAPO_BUFFER_SILENT) {
        outParam.BufferFlags = inParam.BufferFlags;
        return;
    }

    //バッファの取得
    float* inBuf = static_cast<float*>(inParam.pBuffer);
    float* outBuf = static_cast<float*>(outParam.pBuffer);
    //3配列のうち有効な値を取得
    float getFrequency = *reinterpret_cast<float*>(BeginProcess());

    //ローパス係数の計算
    float a1 = expf(-Math::PI * getFrequency); //元の音を残すブレンド率(%)
    float b0 = 1.f - a1; //新しい音のブレンド率(%)

    float volume = mLastVolume;
    for (size_t i = 0; i < inParam.ValidFrameCount; i++) {
        volume = volume * a1 + *inBuf * b0;
        *outBuf = volume;
        ++inBuf;
        ++outBuf;
    }

    mLastVolume = volume;
    outParam.ValidFrameCount = inParam.ValidFrameCount;
    outParam.BufferFlags = inParam.BufferFlags;

    EndProcess();
}
