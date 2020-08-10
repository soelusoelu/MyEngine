#include "SoundPlayTimer.h"
#include "../Effects/SoundEffect.h"
#include "../Voice/SourceVoice/SourceVoice.h"

SoundPlayTimer::SoundPlayTimer(SourceVoice& sourceVoice) :
    mSourceVoice(sourceVoice) {
}

SoundPlayTimer::~SoundPlayTimer() = default;

void SoundPlayTimer::setPlayTime(float time) {
    mSourceVoice.getSoundEffect().setEffectParameters(SoundEffect::PLAY_TIMER_ID, &time, sizeof(time));
}

float SoundPlayTimer::getPlayTime() const {
    float out = 0.f;
    mSourceVoice.getSoundEffect().getEffectParameters(SoundEffect::PLAY_TIMER_ID, &out, sizeof(out));
    return out;
}

void SoundPlayTimer::startTimer() {
    mSourceVoice.getSoundEffect().enable(SoundEffect::PLAY_TIMER_ID);
}

void SoundPlayTimer::stopTimer() {
    mSourceVoice.getSoundEffect().disable(SoundEffect::PLAY_TIMER_ID);
}
