#include "SoundVolume.h"
#include "MasteringVoice.h"
#include "OutputVoiceDetails.h"
#include "SoundFade.h"
#include "SourceVoice.h"
#include "VoiceDetails.h"
#include "../DebugLayer/Debug.h"
#include "../Math/Math.h"

SoundVolume::SoundVolume(SourceVoice& sourceVoice, MasteringVoice& masteringVoice) :
    mSourceVoice(sourceVoice),
    mMasteringVoice(masteringVoice),
    mFader(std::make_unique<SoundFade>(*this)),
    mCurrentVolume(1.f) {
}

SoundVolume::~SoundVolume() = default;

void SoundVolume::update() {
    mFader->updateFade();
}

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

void SoundVolume::pan(float volumes[], unsigned operationSet) {
    const auto inputChannels = mSourceVoice.getSoundData().getInputChannels();
    const auto outputChannels = mMasteringVoice.getDetails().outputChannels;
    auto res = mSourceVoice.getXAudio2SourceVoice()->SetOutputMatrix(NULL, inputChannels, outputChannels, volumes, operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed pan.");
    }
#endif // _DEBUG
}

SoundFade& SoundVolume::fade() const {
    return *mFader;
}
