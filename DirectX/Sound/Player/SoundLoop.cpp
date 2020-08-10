#include "SoundLoop.h"
#include "SoundPlayer.h"
#include "SoundPlayTimer.h"
#include "../Voice/VoiceDetails.h"
#include "../Voice/SourceVoice/SourceVoice.h"

SoundLoop::SoundLoop(SourceVoice& sourceVoice, SoundPlayer& player) :
    mPlayer(player),
    mSourceVoice(sourceVoice),
    mLoopBegin(0.f),
    mLoopEnd(0.f),
    mIsLoop(false) {
}

SoundLoop::~SoundLoop() = default;

void SoundLoop::update() {
    if (!mIsLoop) {
        return;
    }

    //現在の再生時間がループ折返し地点を超えていたらループの開始地点に戻す
    if (mPlayer.playTimer().getPlayTime() >= mLoopEnd) {
        mPlayer.setPlayPoint(mLoopBegin);
    }
}

void SoundLoop::setLoopPoint(float begin, float end) {
    mLoopBegin = begin;
    mLoopEnd = end;
    mIsLoop = true;

    //再生時間内にクランプ
    if (mLoopBegin < 0.f) {
        mLoopBegin = 0.f;
    }
    const auto& data = mSourceVoice.getSoundData();
    auto maxSize = static_cast<float>(data.size) / static_cast<float>(data.averageBytesPerSec);
    if (mLoopEnd > maxSize) {
        mLoopEnd = maxSize;
    }
}

void SoundLoop::exitLoop() {
    mIsLoop = false;
}

constexpr bool SoundLoop::isLoop() const {
    return mIsLoop;
}
