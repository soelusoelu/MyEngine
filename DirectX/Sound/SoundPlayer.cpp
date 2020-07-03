#include "SoundPlayer.h"
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

void SoundPlayer::playInfinity(unsigned flags, unsigned operationSet) const {
    auto& buf = mSourceVoice.getSoundBuffer();
    buf.loopCount = XAUDIO2_LOOP_INFINITE;
    playFromSoundBuffer(buf, flags, operationSet);
}

void SoundPlayer::stop(unsigned flags, unsigned operationSet) const {
    auto res = mSourceVoice.getXAudio2SourceVoice()->Stop(flags, operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed suond stop.");
    }
#endif // _DEBUG
}

bool SoundPlayer::isFinished() const {
    XAUDIO2_VOICE_STATE state;
    mSourceVoice.getXAudio2SourceVoice()->GetState(&state);
    return (state.BuffersQueued == 0);
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
