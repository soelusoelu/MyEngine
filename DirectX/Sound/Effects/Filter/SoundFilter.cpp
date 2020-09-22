#include "SoundFilter.h"
#include "BiQuadFilter.h"
#include "HighPassOnePoleFilter.h"
#include "LowPassOnePoleFilter.h"
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

int SoundFilter::lowPassFilter(float cutoffFrequency, float qualityFactor) {
    int id = mEffectCreater.createEffect(reinterpret_cast<IUnknown*>(new BiQuadFilter(FilterType::LOW_PASS_FILTER)));
    FilterParam param = { cutoffFrequency, qualityFactor };
    mEffectParameter.setEffectParameters(id, &param, sizeof(param));
    return id;
}

int SoundFilter::lowPassOnePoleFilter(float frequency) {
    int id = mEffectCreater.createEffect(reinterpret_cast<IUnknown*>(new MyFilter::LowPassOnePoleFilter()));
    frequency = Math::clamp<float>(frequency, 0.f, 1.f);
    mEffectParameter.setEffectParameters(id, &frequency, sizeof(float));
    return id;
}

int SoundFilter::highPassFilter(float cutoffFrequency, float qualityFactor) {
    int id = mEffectCreater.createEffect(reinterpret_cast<IUnknown*>(new BiQuadFilter(FilterType::HIGH_PASS_FILTER)));
    FilterParam param = { cutoffFrequency, qualityFactor };
    mEffectParameter.setEffectParameters(id, &param, sizeof(param));
    return id;
}

int SoundFilter::highPassOnePoleFilter(float frequency) {
    int id = mEffectCreater.createEffect(reinterpret_cast<IUnknown*>(new MyFilter::HighPassOnePoleFilter()));
    frequency = Math::clamp<float>(frequency, 0.f, 1.f);
    mEffectParameter.setEffectParameters(id, &frequency, sizeof(float));
    return id;
}

void SoundFilter::bandPassFilter(float frequency, float oneOverQ) const {
    setDefaultFilter(XAUDIO2_FILTER_TYPE::BandPassFilter, frequency, oneOverQ, "Failed band pass filter.");
}

void SoundFilter::notchFilter(float frequency, float oneOverQ) const {
    setDefaultFilter(XAUDIO2_FILTER_TYPE::NotchFilter, frequency, oneOverQ, "Failed notch filter.");
}

void SoundFilter::resetFilter() {
    highPassFilter(0.f, 1.f);
}

void SoundFilter::setDefaultFilter(XAUDIO2_FILTER_TYPE type, float frequency, float oneOverQ, const std::string& errorMessage) const {
    if (!mUseFilters) {
        Debug::logWarning("Filter use flag is not set.");
        return;
    }

    auto res = setFilterParameters(type, frequency, oneOverQ);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError(errorMessage);
    }
#endif // _DEBUG
}

HRESULT SoundFilter::setFilterParameters(XAUDIO2_FILTER_TYPE type, float frequency, float oneOverQ) const {
    XAUDIO2_FILTER_PARAMETERS filterParams;
    filterParams.Type = type;
    filterParams.Frequency = frequencyToRadianFrequency(frequency);
    filterParams.OneOverQ = clampOneOverQ(oneOverQ);

    auto res = mVoice.getXAudio2Voice()->SetFilterParameters(&filterParams);
    return res;
}

float SoundFilter::frequencyToRadianFrequency(float frequency) const {
    float f = frequency / mVoice.getVoiceDetails().sampleRate * 6.f;
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
