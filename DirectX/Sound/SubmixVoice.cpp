#include "SubmixVoice.h"
#include "Effects/SoundEffect.h"

SubmixVoice::SubmixVoice(IXAudio2SubmixVoice* XAudio2SubmixVoice, MasteringVoice& masteringVoice, const SubmixVoiceInitParam& param) :
    mXAudio2SubmixVoice(XAudio2SubmixVoice),
    mMasteringVoice(masteringVoice),
    mSoundEffect(std::make_unique<SoundEffect>(*this, masteringVoice)) {
}

SubmixVoice::~SubmixVoice() {
    mXAudio2SubmixVoice->DestroyVoice();
    mXAudio2SubmixVoice = nullptr;
}

IXAudio2Voice* SubmixVoice::getXAudio2Voice() const {
    return mXAudio2SubmixVoice;
}

SoundEffect& SubmixVoice::getSoundEffect() const {
    return *mSoundEffect;
}
