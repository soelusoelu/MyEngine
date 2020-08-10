#include "SoundPlayer.h"
#include "Frequency.h"
#include "SoundLoop.h"
#include "SoundPlayTimer.h"
#include "../Streaming/SoundStreaming.h"
#include "../Voice/VoiceDetails.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../Volume/SoundFade.h"
#include "../Volume/SoundVolume.h"
#include "../../DebugLayer/Debug.h"

SoundPlayer::SoundPlayer(SourceVoice& sourceVoice, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format, float maxFrequencyRatio) :
    mSourceVoice(sourceVoice),
    mStreaming(std::make_unique<SoundStreaming>(sourceVoice, loader, format)),
    mPlayTimer(std::make_unique<SoundPlayTimer>(sourceVoice, *this)),
    mLoop(std::make_unique<SoundLoop>(sourceVoice, *this)),
    mFrequency(std::make_unique<Frequency>(sourceVoice, *this, maxFrequencyRatio)) {
}

SoundPlayer::~SoundPlayer() = default;

void SoundPlayer::update() {
    mLoop->update();
    mStreaming->update();
}

void SoundPlayer::playStreamingFadeIn(float targetVolume, float targetTime) {
    mPlayTimer->startTimer();
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

void SoundPlayer::setPlayPoint(float point) {
    //再生待ちバッファをすべて削除する
    mSourceVoice.getXAudio2SourceVoice()->FlushSourceBuffers();
    mPlayTimer->setPlayTime(point);
    mStreaming->seek(point);
}

void SoundPlayer::pause() const {
    mPlayTimer->stopTimer();
    auto res = mSourceVoice.getXAudio2SourceVoice()->Stop(XAUDIO2_PLAY_TAILS);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed sound pause.");
    }
#endif // _DEBUG
}

void SoundPlayer::pauseFadeOut(float targetTime) const {
    mSourceVoice.getSoundVolume().fade().settings(0.f, targetTime, [&]() { pause(); });
}

void SoundPlayer::stop() const {
    mPlayTimer->stopTimer();
    auto res = mSourceVoice.getXAudio2SourceVoice()->Stop(0);
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

SoundPlayTimer& SoundPlayer::playTimer() const {
    return *mPlayTimer;
}

SoundLoop& SoundPlayer::loop() const {
    return *mLoop;
}

Frequency& SoundPlayer::frequency() const {
    return *mFrequency;
}
