#include "SoundPlayer.h"
#include "Frequency.h"
#include "../Voice/VoiceDetails.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../Volume/SoundFade.h"
#include "../Volume/SoundVolume.h"
#include "../../DebugLayer/Debug.h"

SoundPlayer::SoundPlayer(SourceVoice& sourceVoice, float maxFrequencyRatio) :
    mSourceVoice(sourceVoice),
    mFrequency(std::make_unique<Frequency>(sourceVoice, maxFrequencyRatio)) {
}

SoundPlayer::~SoundPlayer() = default;

void SoundPlayer::play(unsigned operationSet) const {
    SoundBuffer buf;
    //buf.buffer = mSourceVoice.getSoundData().buffer;
    //buf.size = mSourceVoice.getSoundData().size;
    playFromSoundBuffer(buf, operationSet);
}

void SoundPlayer::playFadeIn(float targetVolume, float targetTime, unsigned operationSet) const {
    mSourceVoice.getSoundVolume().setVolume(0.f);
    mSourceVoice.getSoundVolume().fade().settings(targetVolume, targetTime);
    play(operationSet);
}

void SoundPlayer::playInfinity(unsigned operationSet) const {
    SoundBuffer buf;
    //buf.buffer = mSourceVoice.getSoundData().buffer;
    //buf.size = mSourceVoice.getSoundData().size;
    buf.loopCount = XAUDIO2_LOOP_INFINITE;
    playFromSoundBuffer(buf, operationSet);
}

void SoundPlayer::playFromSoundBuffer(const SoundBuffer& buffer, unsigned operationSet) const {
    submitSourceBuffer(buffer);
    auto res = mSourceVoice.getXAudio2SourceVoice()->Start(0, operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed sound play.");
    }
#endif // _DEBUG
}

void SoundPlayer::pause(unsigned flags, unsigned operationSet) const {
    auto res = mSourceVoice.getXAudio2SourceVoice()->Stop(flags, operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed sound pause.");
    }
#endif // _DEBUG
}

void SoundPlayer::pauseFadeOut(float targetTime) const {
    mSourceVoice.getSoundVolume().fade().settings(0.f, targetTime, [&]() { pause(); });
}

void SoundPlayer::stop(unsigned operationSet) const {
    auto res = mSourceVoice.getXAudio2SourceVoice()->Stop(0, operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed sound stop.");
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

void SoundPlayer::submitSourceBuffer(const SoundBuffer& buffer) const {
    auto res = mSourceVoice.getXAudio2SourceVoice()->SubmitSourceBuffer(&toBuffer(buffer), nullptr);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed submit source buffer.");
    }
#endif // _DEBUG
}

Frequency& SoundPlayer::frequency() const {
    return *mFrequency;
}

XAUDIO2_BUFFER SoundPlayer::toBuffer(const SoundBuffer& buffer) const {
    const unsigned sampleRate = mSourceVoice.getVoiceDetails().sampleRate;

    XAUDIO2_BUFFER buf;
    buf.Flags = static_cast<unsigned>(buffer.flag);
    buf.AudioBytes = buffer.size;
    buf.pAudioData = buffer.buffer;
    buf.PlayBegin = static_cast<unsigned>(buffer.playBegin * sampleRate);
    buf.PlayLength = static_cast<unsigned>(buffer.playLength * sampleRate);
    buf.LoopBegin = static_cast<unsigned>(buffer.loopBegin * sampleRate);
    buf.LoopLength = static_cast<unsigned>(buffer.loopLength * sampleRate);
    buf.LoopCount = buffer.loopCount;
    buf.pContext = buffer.context;

    return buf;
}
