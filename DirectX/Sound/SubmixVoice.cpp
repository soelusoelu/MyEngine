#include "SubmixVoice.h"

SubmixVoice::SubmixVoice(IXAudio2SubmixVoice* XAudio2SubmixVoice, const SubmixVoiceInitParam& param) :
    mXAudio2SubmixVoice(XAudio2SubmixVoice)
{
}

SubmixVoice::~SubmixVoice() {
    mXAudio2SubmixVoice->DestroyVoice();
    mXAudio2SubmixVoice = nullptr;
}

IXAudio2SubmixVoice* SubmixVoice::getXAudio2SubmixVoice() const {
    return mXAudio2SubmixVoice;
}
