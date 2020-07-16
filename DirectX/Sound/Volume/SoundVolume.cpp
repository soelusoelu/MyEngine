#include "SoundVolume.h"
#include "SoundFade.h"
#include "SoundPan.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"

SoundVolume::SoundVolume(IVoice& voice, MasteringVoice& masteringVoice, OutputVoices& outputVoices) :
    mVoice(voice),
    mFader(std::make_unique<SoundFade>(*this)),
    mSoundPan(std::make_unique<SoundPan>(voice, masteringVoice, outputVoices)),
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
    auto res = mVoice.getXAudio2Voice()->SetVolume(targetVolume, operationSet);

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

SoundFade& SoundVolume::fade() const {
    return *mFader;
}

SoundPan& SoundVolume::getSoundPan() const {
    return *mSoundPan;
}
