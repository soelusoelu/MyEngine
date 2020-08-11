#include "SoundFilter.h"
#include "LowPass/LowPassOnePoleFilter.h"
#include "HighPass/HighPassOnePoleFilter.h"
#include "../../Voice/VoiceDetails.h"
#include "../../../DebugLayer/Debug.h"
#include "../../../Math/Math.h"

SoundFilter::SoundFilter(IVoice& voice, IEffectCreater& effectCreater, IEffectParameter& effectParameter, bool useFilters) :
    mEffectCreater(effectCreater),
    mEffectParameter(effectParameter),
    mVoice(voice),
    mUseFilters(useFilters) {
}

SoundFilter::~SoundFilter() = default;

void SoundFilter::lowPassFilter(float getFrequency, float oneOverQ, unsigned operationSet) const {
    setDefaultFilter(XAUDIO2_FILTER_TYPE::LowPassFilter, getFrequency, oneOverQ, operationSet, "Failed low pass filter.");
}

int SoundFilter::lowPassOnePoleFilter(float getFrequency) {
    int id = mEffectCreater.createEffect(reinterpret_cast<IUnknown*>(new MyFilter::LowPassOnePoleFilter()));
    getFrequency = Math::clamp<float>(getFrequency, 0.f, 1.f);
    mEffectParameter.setEffectParameters(id, &getFrequency, sizeof(float));
    return id;
}

void SoundFilter::highPassFilter(float getFrequency, float oneOverQ, unsigned operationSet) const {
    setDefaultFilter(XAUDIO2_FILTER_TYPE::HighPassFilter, getFrequency, oneOverQ, operationSet, "Failed high pass filter.");
}

int SoundFilter::highPassOnePoleFilter(float getFrequency) {
    int id = mEffectCreater.createEffect(reinterpret_cast<IUnknown*>(new MyFilter::HighPassOnePoleFilter()));
    getFrequency = Math::clamp<float>(getFrequency, 0.f, 1.f);
    mEffectParameter.setEffectParameters(id, &getFrequency, sizeof(float));
    return id;
}

void SoundFilter::bandPassFilter(float getFrequency, float oneOverQ, unsigned operationSet) const {
    setDefaultFilter(XAUDIO2_FILTER_TYPE::BandPassFilter, getFrequency, oneOverQ, operationSet, "Failed band pass filter.");
}

void SoundFilter::notchFilter(float getFrequency, float oneOverQ, unsigned operationSet) const {
    setDefaultFilter(XAUDIO2_FILTER_TYPE::NotchFilter, getFrequency, oneOverQ, operationSet, "Failed notch filter.");
}

void SoundFilter::resetFilter() {
    highPassFilter(0.f, 1.f);
}

void SoundFilter::setDefaultFilter(XAUDIO2_FILTER_TYPE type, float getFrequency, float oneOverQ, unsigned operationSet, const std::string& errorMessage) const {
    if (!mUseFilters) {
        Debug::logWarning("Filter use flag is not set.");
        return;
    }

    auto res = setFilterParameters(type, getFrequency, oneOverQ, operationSet);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError(errorMessage);
    }
#endif // _DEBUG
}

HRESULT SoundFilter::setFilterParameters(XAUDIO2_FILTER_TYPE type, float getFrequency, float oneOverQ, unsigned operationSet) const {
    XAUDIO2_FILTER_PARAMETERS filterParams;
    filterParams.Type = type;
    filterParams.Frequency = frequencyToRadianFrequency(getFrequency);
    filterParams.OneOverQ = clampOneOverQ(oneOverQ);

    auto res = mVoice.getXAudio2Voice()->SetFilterParameters(&filterParams, operationSet);
    return res;
}

float SoundFilter::frequencyToRadianFrequency(float getFrequency) const {
    float f = getFrequency / mVoice.getVoiceDetails().sampleRate * 6.f;
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
