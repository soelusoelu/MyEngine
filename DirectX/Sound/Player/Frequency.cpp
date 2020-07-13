#include "Frequency.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../../DebugLayer/Debug.h"

Frequency::Frequency(SourceVoice& sourceVoice, float maxFrequencyRatio) :
    mSourceVoice(sourceVoice),
    mMaxFrequencyRatio(maxFrequencyRatio) {
}

Frequency::~Frequency() = default;

void Frequency::setFrequencyRatio(float pitch, unsigned operationSet) {
    if (pitch > mMaxFrequencyRatio) {
        pitch = mMaxFrequencyRatio;
        Debug::logWarning("Pitch is over max frequency.");
    }

    auto res = mSourceVoice.getXAudio2SourceVoice()->SetFrequencyRatio(pitch, operationSet);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed change frequency ratio.");
    }
#endif // _DEBUG
}
