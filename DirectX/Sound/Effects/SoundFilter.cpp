#include "SoundFilter.h"
#include "../Voice/VoiceDetails.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"

SoundFilter::SoundFilter(SourceVoice& sourceVoice, bool useFilters) :
    mSourceVoice(sourceVoice),
    mUseFilters(useFilters) {
}

SoundFilter::~SoundFilter() = default;

void SoundFilter::lowPassFilter(float frequency, float oneOverQ, unsigned operationSet) const {
    if (!mUseFilters) {
        printWarnigUseFilters();
        return;
    }

    auto res = setFilterParameters(XAUDIO2_FILTER_TYPE::LowPassFilter, frequency, oneOverQ, operationSet);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed low pass filter.");
    }
#endif // _DEBUG
}

void SoundFilter::highPassFilter(float frequency, float oneOverQ, unsigned operationSet) const {
    if (!mUseFilters) {
        printWarnigUseFilters();
        return;
    }

    auto res = setFilterParameters(XAUDIO2_FILTER_TYPE::HighPassFilter, frequency, oneOverQ, operationSet);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed high pass filter.");
    }
#endif // _DEBUG
}

void SoundFilter::bandPassFilter(float frequency, float oneOverQ, unsigned operationSet) const {
    if (!mUseFilters) {
        printWarnigUseFilters();
        return;
    }

    auto res = setFilterParameters(XAUDIO2_FILTER_TYPE::BandPassFilter, frequency, oneOverQ, operationSet);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed band pass filter.");
    }
#endif // _DEBUG
}

void SoundFilter::notchFilter(float frequency, float oneOverQ, unsigned operationSet) const {
    if (!mUseFilters) {
        printWarnigUseFilters();
        return;
    }

    auto res = setFilterParameters(XAUDIO2_FILTER_TYPE::NotchFilter, frequency, oneOverQ, operationSet);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed notch filter.");
    }
#endif // _DEBUG
}

void SoundFilter::resetFilter() {
    highPassFilter(0.f, 1.f);
}

HRESULT SoundFilter::setFilterParameters(XAUDIO2_FILTER_TYPE type, float frequency, float oneOverQ, unsigned operationSet) const {
    XAUDIO2_FILTER_PARAMETERS filterParams;
    filterParams.Type = type;
    filterParams.Frequency = frequencyToRadianFrequency(frequency);
    filterParams.OneOverQ = clampOneOverQ(oneOverQ);

    auto res = mSourceVoice.getXAudio2SourceVoice()->SetFilterParameters(&filterParams, operationSet);
    return res;
}

float SoundFilter::frequencyToRadianFrequency(float frequency) const {
    float f = frequency / mSourceVoice.getVoiceDetails().samplesPerSec * 6.f;
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

void SoundFilter::printWarnigUseFilters() const {
    Debug::logWarning("Filter use flag is not set.");
}
