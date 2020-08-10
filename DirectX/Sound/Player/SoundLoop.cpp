#include "SoundLoop.h"
#include "SoundPlayer.h"
#include "SoundPlayTimer.h"
#include "../../DebugLayer/Debug.h"

SoundLoop::SoundLoop(SoundPlayer& player, SoundPlayTimer& playTimer, const SoundData& data) :
    mPlayer(player),
    mPlayTimer(playTimer),
    mData(data),
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
    if (mPlayTimer.getPlayTime() >= mLoopEnd) {
        mPlayer.setPlayPoint(mLoopBegin);
        Debug::log("loop");
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
    auto maxSize = static_cast<float>(mData.size) / static_cast<float>(mData.averageBytesPerSec);
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
