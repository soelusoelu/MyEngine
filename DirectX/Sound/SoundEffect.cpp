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

    float f = frequency / mSourceVoice.getSoundData().getSampleRate() * 6.f;
    filterParams.Frequency = Math::clamp<float>(f, 0.f, XAUDIO2_MAX_FILTER_FREQUENCY);

    if (oneOverQ <= 0.f) {
        oneOverQ = 0.000001f;
    } else if (oneOverQ > XAUDIO2_MAX_FILTER_ONEOVERQ) {
        oneOverQ = XAUDIO2_MAX_FILTER_ONEOVERQ;
    }
    filterParams.OneOverQ = oneOverQ;

    auto res = mSourceVoice.getXAudio2SourceVoice()->SetFilterParameters(&filterParams, operationSet);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed low pass filter.");
    }
#endif // _DEBUG
}

void SoundEffect::resetLowPassFilter() {
    float max = mSourceVoice.getSoundData().getSampleRate() / 6.f;
    lowPassFilter(max, 1.f);
}
