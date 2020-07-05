#include "SoundEffect.h"
#include "SourceVoice.h"
#include "VoiceDetails.h"
#include "../DebugLayer/Debug.h"
#include "../Math/Math.h"
#include <xaudio2.h>

SoundEffect::SoundEffect(SourceVoice& sourceVoice) :
    mSourceVoice(sourceVoice) {
}

SoundEffect::~SoundEffect() = default;

void SoundEffect::lowPassFilter(float frequency, float oneOverQ, unsigned operationSet) const {
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
