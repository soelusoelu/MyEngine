#include "SoundVolume.h"
#include "SoundFade.h"
#include "SoundPan.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"

SoundVolume::SoundVolume(IVoice& voice, const unsigned inChannels, const unsigned outChannels) :
    mVoice(voice),
    mFader(std::make_unique<SoundFade>(*this)),
    mSoundPan(std::make_unique<SoundPan>(voice, inChannels, outChannels)),
    mCurrentVolume(1.f) {
}

SoundVolume::~SoundVolume() = default;

void SoundVolume::update() {
    mFader->update();
}

void SoundVolume::setVolume(float volume) {
    //前回とほぼ同じ値だったら変更しない
    if (Math::nearZero(mCurrentVolume - volume)) {
        return;
    }

    mCurrentVolume = volume;
    float targetVolume = mCurrentVolume * mCurrentVolume;
    auto res = mVoice.getXAudio2Voice()->SetVolume(targetVolume);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed volume setting.");
    }
#endif // _DEBUG
}

void SoundVolume::setVolumeByDecibels(float decibels) {
    float volume = decibelsToAmplitudeRatio(decibels);
    setVolume(volume);
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

SoundFade& SoundVolume::getFade() const {
    return *mFader;
}

SoundPan& SoundVolume::getPan() const {
    return *mSoundPan;
}
