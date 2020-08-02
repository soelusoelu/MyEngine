#include "SubmixVoice.h"
#include "../../Effects/SoundEffect.h"
#include "../../Flag/SoundFlag.h"
#include "../../Output/OutputVoices.h"
#include "../../Volume/SoundVolume.h"
#include "../../../Device/Flag.h"

SubmixVoice::SubmixVoice(IXAudio2SubmixVoice* XAudio2SubmixVoice, MasteringVoice& masteringVoice, const SubmixVoiceInitParam& param) :
    mXAudio2SubmixVoice(XAudio2SubmixVoice),
    mDetails(),
    mSoundVolume(nullptr),
    mOutputVoices(std::make_unique<OutputVoices>(*this)),
    mSoundEffect(std::make_unique<SoundEffect>(*this, masteringVoice, param.flags.check(static_cast<unsigned>(SoundFlag::USE_FILTER)))) {

    mDetails.inputChannels = param.inputChannels;
    mDetails.samplesPerSec = param.inputSampleRate;

    mSoundVolume = std::make_unique<SoundVolume>(*this, masteringVoice, *mOutputVoices);
}

SubmixVoice::~SubmixVoice() {
    mXAudio2SubmixVoice->DestroyVoice();
    mXAudio2SubmixVoice = nullptr;
}

IXAudio2Voice* SubmixVoice::getXAudio2Voice() const {
    return mXAudio2SubmixVoice;
}

const VoiceDetails& SubmixVoice::getVoiceDetails() const {
    return mDetails;
}

SoundVolume& SubmixVoice::getSoundVolume() const {
    return *mSoundVolume;
}

SoundEffect& SubmixVoice::getSoundEffect() const {
    return *mSoundEffect;
}

OutputVoices& SubmixVoice::getOutputVoices() const {
    return *mOutputVoices;
}
