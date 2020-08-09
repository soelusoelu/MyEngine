#include "SoundLoop.h"
#include "../Streaming/SoundStreaming.h"
#include "../Voice/VoiceDetails.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../../DebugLayer/Debug.h"
#include <cassert>

SoundLoop::SoundLoop(SourceVoice& sourceVoice, SoundStreaming& streaming) :
    mSourceVoice(sourceVoice),
    mStreaming(streaming),
    mLoopBegin(0.f),
    mLoopEnd(0.f),
    mIsLoop(false) {
}

SoundLoop::~SoundLoop() = default;

void SoundLoop::update() {
    if (!mIsLoop) {
        return;
    }

    const auto next = mStreaming.getNextReadPointInByte();
    const auto bytePerSec = mSourceVoice.getSoundData().averageBytesPerSec;
    //次の読み込み位置がループの折り返し地点を超えていたら
    if (next > mLoopEnd * bytePerSec) {
        mStreaming.seek(mLoopBegin);
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
    const auto& data = mSourceVoice.getSoundData();
    auto maxSize = data.size * data.averageBytesPerSec;
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
