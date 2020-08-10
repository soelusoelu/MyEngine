#include "Frequency.h"
#include "SoundPlayer.h"
#include "SoundPlayTimer.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"
#include <cassert>

Frequency::Frequency(SourceVoice& sourceVoice, SoundPlayer& player, float maxFrequencyRatio) :
    mSourceVoice(sourceVoice),
    mPlayer(player),
    mMaxFrequencyRatio(maxFrequencyRatio),
    mCurrentFrequencyRatio(1.f) {
}

Frequency::~Frequency() = default;

void Frequency::setFrequencyRatio(float pitch) {
    mCurrentFrequencyRatio = pitch;

    if (mCurrentFrequencyRatio > mMaxFrequencyRatio) {
        mCurrentFrequencyRatio = mMaxFrequencyRatio;
        Debug::logWarning("Pitch is over max frequency.");
    }

    auto res = mSourceVoice.getXAudio2SourceVoice()->SetFrequencyRatio(mCurrentFrequencyRatio);
    //再生時間にも影響するので
    mPlayer.playTimer().setFrequency(mCurrentFrequencyRatio);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed change frequency ratio.");
    }
#endif // _DEBUG
}

float Frequency::getFrequencyRatio() const {
    //デバッグ時のみ一応確認
#ifdef _DEBUG
    float pitch = 0.f;
    mSourceVoice.getXAudio2SourceVoice()->GetFrequencyRatio(&pitch);
    assert(Math::equal(pitch, mCurrentFrequencyRatio));
#endif // _DEBUG

    return mCurrentFrequencyRatio;
}
