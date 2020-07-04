﻿#include "SoundFade.h"
#include "SoundVolume.h"
#include "../Device/Time.h"
#include "../Math/Math.h"

SoundFade::SoundFade(SoundVolume& soundVolume) :
    mSoundVolume(soundVolume),
    mFadeEndFunc(nullptr),
    mTargetVolume(0.f),
    mTargetTime(0.f),
    mBeforeVolume(0.f),
    mTimeRate(0.f),
    mIsFading(false) {
}

SoundFade::~SoundFade() = default;

void SoundFade::settings(float targetVolume, float targetTime, const std::function<void()>& f) {
    mTargetVolume = targetVolume;
    mTargetTime = targetTime;
    mFadeEndFunc = f;
    mBeforeVolume = mSoundVolume.getVolume();
    mTimeRate = 0.f;
    mIsFading = true;
}

void SoundFade::updateFade() {
    if (!mIsFading) {
        return;
    }

    mTimeRate += Time::deltaTime / mTargetTime;
    if (mTimeRate >= 1.f) {
        mTimeRate = 1.f;
        mIsFading = false;
    }
    auto nextVolume = Math::lerp(mBeforeVolume, mTargetVolume, mTimeRate);
    mSoundVolume.setVolume(nextVolume);

    //フェードを終了するタイミングで
    if (!mIsFading) {
        if (mFadeEndFunc) {
            mFadeEndFunc();
        }
    }
}

bool SoundFade::isFading() const {
    return mIsFading;
}
