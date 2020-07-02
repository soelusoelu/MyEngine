#include "Sound.h"
#include "SoundLoader.h"
#include "SourceVoice.h"
#include "../DebugLayer/Debug.h"

Sound::Sound() :
    mSourceVoice(nullptr),
    mData(nullptr) {
}

Sound::~Sound() = default;

void Sound::play(bool isLoop) {
    SoundBuffer buffer;
    buffer.buffer = mData->buffer();
    buffer.flags = XAUDIO2_END_OF_STREAM;
    buffer.size = mData->size();
    if (isLoop) {
        buffer.loopCount = XAUDIO2_LOOP_INFINITE;
    }
    mSourceVoice->submitSourceBuffer(buffer);
    mSourceVoice->play();
}

void Sound::stop() const {
    mSourceVoice->stop();
}

bool Sound::isFinished() const {
    XAUDIO2_VOICE_STATE state;
    mSourceVoice->GetState(&state);
    return (state.BuffersQueued == 0);
}

void Sound::setVolume(float volume) const {
    float targetVolume = volume * volume;
    mSourceVoice->setVolume(targetVolume);
}

void Sound::setVolumeByDecibels(float decibels) const {
    float volume = decibelsToAmplitudeRatio(decibels);
    mSourceVoice->setVolume(volume);
}

float Sound::amplitudeToDecibelsRatio(float volume) const {
    return XAudio2AmplitudeRatioToDecibels(volume);
}

float Sound::decibelsToAmplitudeRatio(float decibels) const {
    return XAudio2DecibelsToAmplitudeRatio(decibels);
}
