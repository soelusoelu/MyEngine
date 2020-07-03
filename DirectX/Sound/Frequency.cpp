#include "Frequency.h"
#include "SourceVoice.h"
#include "../DebugLayer/Debug.h"

Frequency::Frequency(SourceVoice& sourceVoice) :
    mSourceVoice(sourceVoice) {
}

Frequency::~Frequency() = default;

void Frequency::setFrequencyRatio(float pitch, unsigned operationSet) {
    auto res = mSourceVoice.getXAudio2SourceVoice()->SetFrequencyRatio(pitch, operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed change frequency ratio.");
    }
#endif // _DEBUG
}
