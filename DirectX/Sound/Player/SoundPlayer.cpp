#include "SoundPlayer.h"
#include "Frequency.h"
#include "../Streaming/SoundStreaming.h"
#include "../Voice/VoiceDetails.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../Volume/SoundFade.h"
#include "../Volume/SoundVolume.h"
#include "../../DebugLayer/Debug.h"

SoundPlayer::SoundPlayer(SourceVoice& sourceVoice, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format, float maxFrequencyRatio) :
    mSourceVoice(sourceVoice),
    mStreaming(std::make_unique<SoundStreaming>(sourceVoice, loader, format)),
    mFrequency(std::make_unique<Frequency>(sourceVoice, maxFrequencyRatio)) {
}

SoundPlayer::~SoundPlayer() = default;

void SoundPlayer::update() {
    mStreaming->update();
}

void SoundPlayer::playStreamingFadeIn(float targetVolume, float targetTime) {
    mSourceVoice.getSoundVolume().setVolume(0.f);
    mSourceVoice.getSoundVolume().fade().settings(targetVolume, targetTime);
    mStreaming->play();
    auto res = mSourceVoice.getXAudio2SourceVoice()->Start();
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed play streaming.");
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

Frequency& SoundPlayer::frequency() const {
    return *mFrequency;
}
