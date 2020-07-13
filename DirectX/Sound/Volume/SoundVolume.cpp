#include "SoundVolume.h"
#include "SoundFade.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../Voice/MasteringVoice/OutputVoiceDetails.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../Voice/SourceVoice/VoiceDetails.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"
#include "../../System/Window.h"
#include <vector>

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

void SoundVolume::pan(float positionX, unsigned operationSet) {
    const unsigned inputChannels = mSourceVoice.getSoundData().getInputChannels();
    const unsigned outputChannels = mMasteringVoice.getDetails().outputChannels;
    const float width = static_cast<float>(Window::standardWidth());

    auto posX = Math::clamp<float>(positionX, 0.f, width);
    float rot = posX / width * 90.f;
    std::vector<float> volumes(inputChannels * outputChannels);
    volumes[0] = volumes[1] = Math::cos(rot);
    volumes[2] = volumes[3] = Math::sin(rot);

    auto res = mSourceVoice.getXAudio2SourceVoice()->SetOutputMatrix(nullptr, inputChannels, outputChannels, volumes.data(), operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed pan.");
    }
#endif // _DEBUG
}

SoundFade& SoundVolume::fade() const {
    return *mFader;
}
