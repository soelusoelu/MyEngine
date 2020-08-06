#include "SourceVoice.h"
#include "../SubmixVoice/SubmixVoice.h"
#include "../../Effects/SoundEffect.h"
#include "../../Flag/SoundFlag.h"
#include "../../Player/SoundPlayer.h"
#include "../../Voice/Output/OutputVoices.h"
#include "../../Volume/SoundVolume.h"
#include "../../../DebugLayer/Debug.h"
#include "../../../Device/Flag.h"

SourceVoice::SourceVoice(IXAudio2SourceVoice* XAudio2SourceVoice, MasteringVoice& masteringVoice, const WaveFormat& format, const SourceVoiceInitParam& param) :
    mXAudio2SourceVoice(XAudio2SourceVoice),
    mDetails(),
    mSoundData(std::make_unique<SoundData>(format)),
    mSoundPlayer(std::make_unique<SoundPlayer>(*this, param.maxFrequencyRatio)),
    mSoundVolume(nullptr),
    mOutputVoices(std::make_unique<OutputVoices>(*this)),
    mSoundEffect(std::make_unique<SoundEffect>(*this, param.flags.check(static_cast<unsigned>(SoundFlag::USE_FILTER)))) {

    mDetails.channels = format.channels;
    mDetails.sampleRate = format.samplesPerSec;

    mSoundVolume = std::make_unique<SoundVolume>(*this, masteringVoice, *mOutputVoices);
}

SourceVoice::~SourceVoice() {
    mXAudio2SourceVoice->DestroyVoice();
    mXAudio2SourceVoice = nullptr;
}

IXAudio2Voice* SourceVoice::getXAudio2Voice() const {
    return mXAudio2SourceVoice;
}

const VoiceDetails& SourceVoice::getVoiceDetails() const {
    return mDetails;
}

SoundVolume& SourceVoice::getSoundVolume() const {
    return *mSoundVolume;
}

OutputVoices& SourceVoice::getOutputVoices() const {
    return *mOutputVoices;
}

SoundEffect& SourceVoice::getSoundEffect() const {
    return *mSoundEffect;
}

void SourceVoice::update() {
    mSoundVolume->update();
}

IXAudio2SourceVoice* SourceVoice::getXAudio2SourceVoice() const {
    return mXAudio2SourceVoice;
}

SoundData& SourceVoice::getSoundData() const {
    return *mSoundData;
}

SoundPlayer& SourceVoice::getSoundPlayer() const {
    return *mSoundPlayer;
}
