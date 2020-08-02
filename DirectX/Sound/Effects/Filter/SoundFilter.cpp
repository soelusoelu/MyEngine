#include "SoundFilter.h"
#include "LowPass/LowPassOnePoleFilter.h"
#include "HighPass/HighPassOnePoleFilter.h"
#include "../../Voice/VoiceDetails.h"
#include "../../../DebugLayer/Debug.h"
#include "../../../Math/Math.h"

SoundFilter::SoundFilter(IEffectCreater& effectCreater, IEffectParameter& effectParameter, IVoice& voice, bool useFilters) :
    mEffectCreater(effectCreater),
    mEffectParameter(effectParameter),
    mVoice(voice),
    mUseFilters(useFilters) {
}

SoundFilter::~SoundFilter() = default;

void SoundFilter::lowPassFilter(float frequency, float oneOverQ, unsigned operationSet) const {
    setDefaultFilter(XAUDIO2_FILTER_TYPE::LowPassFilter, frequency, oneOverQ, operationSet, "Failed low pass filter.");
}

int SoundFilter::lowPassOnePoleFilter(float frequency) {
    int id = mEffectCreater.createEffect(reinterpret_cast<IUnknown*>(new MyFilter::LowPassOnePoleFilter()));
    frequency = Math::clamp<float>(frequency, 0.f, 1.f);
    mEffectParameter.setEffectParameters(id, &frequency, sizeof(float));
    return id;
}

void SoundFilter::highPassFilter(float frequency, float oneOverQ, unsigned operationSet) const {
    setDefaultFilter(XAUDIO2_FILTER_TYPE::HighPassFilter, frequency, oneOverQ, operationSet, "Failed high pass filter.");
}

int SoundFilter::highPassOnePoleFilter(float frequency) {
    int id = mEffectCreater.createEffect(reinterpret_cast<IUnknown*>(new MyFilter::HighPassOnePoleFilter()));
    frequency = Math::clamp<float>(frequency, 0.f, 1.f);
    mEffectParameter.setEffectParameters(id, &frequency, sizeof(float));
    return id;
}

void SoundFilter::bandPassFilter(float frequency, float oneOverQ, unsigned operationSet) const {
    setDefaultFilter(XAUDIO2_FILTER_TYPE::BandPassFilter, frequency, oneOverQ, operationSet, "Failed band pass filter.");
}

void SoundFilter::notchFilter(float frequency, float oneOverQ, unsigned operationSet) const {
    setDefaultFilter(XAUDIO2_FILTER_TYPE::NotchFilter, frequency, oneOverQ, operationSet, "Failed notch filter.");
}

void SoundFilter::resetFilter() {
    highPassFilter(0.f, 1.f);
}

void SoundFilter::setDefaultFilter(XAUDIO2_FILTER_TYPE type, float frequency, float oneOverQ, unsigned operationSet, const std::string& errorMessage) const {
    if (!mUseFilters) {
        Debug::logWarning("Filter use flag is not set.");
        return;
    }

    auto res = setFilterParameters(type, frequency, oneOverQ, operationSet);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError(errorMessage);
    }
#endif // _DEBUG
}

HRESULT SoundFilter::setFilterParameters(XAUDIO2_FILTER_TYPE type, float frequency, float oneOverQ, unsigned operationSet) const {
    XAUDIO2_FILTER_PARAMETERS filterParams;
    filterParams.Type = type;
    filterParams.Frequency = frequencyToRadianFrequency(frequency);
    filterParams.OneOverQ = clampOneOverQ(oneOverQ);

    auto res = mVoice.getXAudio2Voice()->SetFilterParameters(&filterParams, operationSet);
    return res;
}

float SoundFilter::frequencyToRadianFrequency(float frequency) const {
    float f = frequency / mVoice.getVoiceDetails().samplesPerSec * 6.f;
    return Math::clamp<float>(f, 0.f, XAUDIO2_MAX_FILTER_FREQUENCY);
}

float SoundFilter::clampOneOverQ(float oneOverQ) const {
    float f = oneOverQ;
    if (oneOverQ <= 0.f) { //0ちょうどでもだめ
        f = 0.001f;
    } else if (oneOverQ > XAUDIO2_MAX_FILTER_ONEOVERQ) {
        f = XAUDIO2_MAX_FILTER_ONEOVERQ;
    }
    return f;
}
