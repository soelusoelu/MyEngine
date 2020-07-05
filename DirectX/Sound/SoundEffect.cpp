#include "SoundEffect.h"
#include "SourceVoice.h"
#include "VoiceDetails.h"
#include "../DebugLayer/Debug.h"
#include "../Math/Math.h"
#include <xaudio2.h>

SoundEffect::SoundEffect(SourceVoice& sourceVoice, bool useEffects) :
    mSourceVoice(sourceVoice),
    mUseEffects(useEffects) {
}

SoundEffect::~SoundEffect() = default;

void SoundEffect::lowPassFilter(float frequency, float oneOverQ, unsigned operationSet) const {
    if (!mUseEffects) {
        printWarnigUseEffects();
        return;
    }

    XAUDIO2_FILTER_PARAMETERS filterParams;
    filterParams.Type = XAUDIO2_FILTER_TYPE::LowPassFilter;
    filterParams.Frequency = frequencyToRadianFrequency(frequency);
    filterParams.OneOverQ = clampOneOverQ(oneOverQ);

    auto res = mSourceVoice.getXAudio2SourceVoice()->SetFilterParameters(&filterParams, operationSet);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed low pass filter.");
    }
#endif // _DEBUG
}

void SoundEffect::highPassFilter(float frequency, float oneOverQ, unsigned operationSet) const {
    if (!mUseEffects) {
        printWarnigUseEffects();
        return;
    }

    XAUDIO2_FILTER_PARAMETERS filterParams;
    filterParams.Type = XAUDIO2_FILTER_TYPE::HighPassFilter;
    filterParams.Frequency = frequencyToRadianFrequency(frequency);
    filterParams.OneOverQ = clampOneOverQ(oneOverQ);

    auto res = mSourceVoice.getXAudio2SourceVoice()->SetFilterParameters(&filterParams, operationSet);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed high pass filter.");
    }
#endif // _DEBUG
}

void SoundEffect::bandPassFilter(float frequency, float oneOverQ, unsigned operationSet) const {
    if (!mUseEffects) {
        printWarnigUseEffects();
        return;
    }

    XAUDIO2_FILTER_PARAMETERS filterParams;
    filterParams.Type = XAUDIO2_FILTER_TYPE::BandPassFilter;
    filterParams.Frequency = frequencyToRadianFrequency(frequency);
    filterParams.OneOverQ = clampOneOverQ(oneOverQ);

    auto res = mSourceVoice.getXAudio2SourceVoice()->SetFilterParameters(&filterParams, operationSet);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed band pass filter.");
    }
#endif // _DEBUG
}

void SoundEffect::notchFilter(float frequency, float oneOverQ, unsigned operationSet) const {
    if (!mUseEffects) {
        printWarnigUseEffects();
        return;
    }

    XAUDIO2_FILTER_PARAMETERS filterParams;
    filterParams.Type = XAUDIO2_FILTER_TYPE::NotchFilter;
    filterParams.Frequency = frequencyToRadianFrequency(frequency);
    filterParams.OneOverQ = clampOneOverQ(oneOverQ);

    auto res = mSourceVoice.getXAudio2SourceVoice()->SetFilterParameters(&filterParams, operationSet);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed notch filter.");
    }
#endif // _DEBUG
}

void SoundEffect::resetFilter() {
    highPassFilter(0.f, 1.f);
}

float SoundEffect::frequencyToRadianFrequency(float frequency) const {
    float f = frequency / mSourceVoice.getSoundData().getSampleRate() * 6.f;
    return Math::clamp<float>(f, 0.f, XAUDIO2_MAX_FILTER_FREQUENCY);
}

float SoundEffect::clampOneOverQ(float oneOverQ) const {
    float f = oneOverQ;
    if (oneOverQ <= 0.f) { //0ちょうどでもだめ
        f = 0.001f;
    } else if (oneOverQ > XAUDIO2_MAX_FILTER_ONEOVERQ) {
        f = XAUDIO2_MAX_FILTER_ONEOVERQ;
    }
    return f;
}

void SoundEffect::printWarnigUseEffects() const {
#ifdef _DEBUG
    Debug::logWarning("Effect use flag is not set.");
#endif // _DEBUG
}
