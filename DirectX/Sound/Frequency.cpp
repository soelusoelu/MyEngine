#include "Frequency.h"
#include "SourceVoice.h"

Frequency::Frequency(SourceVoice& sourceVoice) :
    mSourceVoice(sourceVoice) {
}

Frequency::~Frequency() = default;

void Frequency::setFrequencyRatio(float pitch, unsigned operationSet) {
    mSourceVoice.getXAudio2SourceVoice()->SetFrequencyRatio(pitch, operationSet);
}
