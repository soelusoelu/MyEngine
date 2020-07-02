#include "SoundVolume.h"
#include "SoundFade.h"
#include "SourceVoice.h"
#include "VoiceDetails.h"
#include "../DebugLayer/Debug.h"
#include "../Math/Math.h"

SoundVolume::SoundVolume(SourceVoice& sourceVoice) :
    mSourceVoice(sourceVoice),
    mFader(std::make_unique<SoundFade>(*this)),
    mCurrentVolume(1.f) {
}

SoundVolume::~SoundVolume() = default;

void SoundVolume::setVolume(float volume, unsigned operationSet) {
    //前回とほぼ同じ値だったら変更しない
    if (Math::nearZero(mCurrentVolume - volume)) {
        return;
    }

    mCurrentVolume = volume;
    float targetVolume = mCurrentVolume * mCurrentVolume;
    auto res = mSourceVoice.getXAudio2SourceVoice()->SetVolume(targetVolume, operationSet);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed volume setting.");
    }
#endif // _DEBUG
}

void SoundVolume::setVolumeByDecibels(float decibels, unsigned operationSet) {
    float volume = decibelsToAmplitudeRatio(decibels);
    setVolume(volume, operationSet);
}

float SoundVolume::amplitudeRatioToDecibels(float volume) {
    return XAudio2AmplitudeRatioToDecibels(volume);
}

float SoundVolume::decibelsToAmplitudeRatio(float decibels) {
    return XAudio2DecibelsToAmplitudeRatio(decibels);
}

float SoundVolume::getVolume() const {
    return mCurrentVolume;
}

void SoundVolume::panning(unsigned outChannels, float volumes[], unsigned operationSet) {
    auto inputChannels = mSourceVoice.getSoundData().getInputChannels();
    auto res = mSourceVoice.getXAudio2SourceVoice()->SetOutputMatrix(nullptr, inputChannels, outChannels, volumes, operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed panning.");
    }
#endif // _DEBUG
}

SoundFade& SoundVolume::fade() const {
    return *mFader;
}
