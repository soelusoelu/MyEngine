#include "SoundPlayer.h"
#include "SoundFade.h"
#include "SoundVolume.h"
#include "SourceVoice.h"
#include "../DebugLayer/Debug.h"

SoundPlayer::SoundPlayer(SourceVoice& sourceVoice) :
    mSourceVoice(sourceVoice) {
}

SoundPlayer::~SoundPlayer() = default;

void SoundPlayer::play(unsigned flags, unsigned operationSet) const {
    const auto& buf = mSourceVoice.getSoundBuffer();
    playFromSoundBuffer(buf, flags, operationSet);
}

void SoundPlayer::playFadeIn(float targetVolume, float targetTime, unsigned flags, unsigned operationSet) const {
    mSourceVoice.getSoundVolume().setVolume(0.f);
    mSourceVoice.getSoundVolume().fade().settings(targetVolume, targetTime);
    play(flags, operationSet);
}

void SoundPlayer::playInfinity(unsigned flags, unsigned operationSet) const {
    auto& buf = mSourceVoice.getSoundBuffer();
    buf.loopCount = XAUDIO2_LOOP_INFINITE;
    playFromSoundBuffer(buf, flags, operationSet);
}

void SoundPlayer::pause(unsigned flags, unsigned operationSet) const {
    auto res = mSourceVoice.getXAudio2SourceVoice()->Stop(flags, operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed suond pause.");
    }
#endif // _DEBUG
}

void SoundPlayer::pauseFadeOut(float targetTime) const {
    mSourceVoice.getSoundVolume().fade().settings(0.f, targetTime, [&]() { pause(); });
}

void SoundPlayer::stop(unsigned flags, unsigned operationSet) const {
    auto res = mSourceVoice.getXAudio2SourceVoice()->Stop(flags, operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed suond stop.");
    }
#endif // _DEBUG
}

void SoundPlayer::stopFadeOut(float targetTime) const {
    mSourceVoice.getSoundVolume().fade().settings(0.f, targetTime, [&]() { stop(); });
}

bool SoundPlayer::isStop() const {
    XAUDIO2_VOICE_STATE state;
    mSourceVoice.getXAudio2SourceVoice()->GetState(&state);
    return (state.BuffersQueued == 0);
}

void SoundPlayer::exitLoop(unsigned operationSet) const {
    auto res = mSourceVoice.getXAudio2SourceVoice()->ExitLoop(operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed exit loop.");
    }
#endif // _DEBUG
}

void SoundPlayer::playFromSoundBuffer(const SoundBuffer& buffer, unsigned flags, unsigned operationSet) const {
    mSourceVoice.submitSourceBuffer(buffer);
    auto res = mSourceVoice.getXAudio2SourceVoice()->Start(flags, operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed sound play.");
    }
#endif // _DEBUG
}
