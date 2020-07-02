#include "SoundFade.h"
#include "../Device/Time.h"
#include "../Math/Math.h"

SoundFade::SoundFade() :
    mTargetVolume(0.f),
    mTargetTime(0.f),
    mBeforeVolume(0.f),
    mTimeRate(0.f),
    mIsFading(false) {
}

SoundFade::~SoundFade() = default;

void SoundFade::fade(float targetVolume, float targetTime, float currentVolume) {
    mTargetVolume = targetVolume;
    mTargetTime = targetTime;
    mBeforeVolume = currentVolume;
    mTimeRate = 0.f;
    mIsFading = true;
}

void SoundFade::updateFade() {
    if (!mIsFading) {
        return;
    }
    mTimeRate += Time::deltaTime / mTargetTime;
    Math::lerp(mBeforeVolume, mTargetVolume, mTimeRate);

    if (mTimeRate >= 1.f) {
        mIsFading = false;
    }
}
