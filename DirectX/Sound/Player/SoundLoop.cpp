#include "SoundLoop.h"
#include "SoundPlayer.h"
#include "SoundPlayTimer.h"
#include "../Data/SoundData.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"

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
    if (mPlayer.playTimer().getPlayTime() >= mLoopEnd - 0.01f) {
        mPlayer.setPlayPoint(mLoopBegin);
    }
}

void SoundLoop::setLoopPoint(float begin, float end) {
    const auto& data = mSourceVoice.getSoundData();

    //ループの開始が曲の長さを超えてたら
    if (begin >= data.length()) {
        Debug::logWarning("The loop start exceeds the length of the song.");
        return;
    }

    mLoopBegin = begin;
    mLoopEnd = end;
    mIsLoop = true;

    //再生時間内にクランプ
    data.clamp(mLoopBegin);
    //折返しの値が0なら曲の終わりまで
    if (Math::nearZero(end)) {
        mLoopEnd = data.length();
    } else {
        data.clamp(mLoopEnd);
    }
}

void SoundLoop::loopAll() {
    mLoopBegin = 0.f;
    mLoopEnd = mSourceVoice.getSoundData().length();
    mIsLoop = true;
}

void SoundLoop::exitLoop() {
    mIsLoop = false;
}

bool SoundLoop::isLoop() const {
    return mIsLoop;
}
