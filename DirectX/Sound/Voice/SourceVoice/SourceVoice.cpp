#include "SourceVoice.h"
#include "../SubmixVoice/SubmixVoice.h"
#include "../../Effects/SoundEffect.h"
#include "../../Effects/SoundEffectCollection.h"
#include "../../Flag/SoundFlag.h"
#include "../../Player/SoundPlayer.h"
#include "../../Voice/VoiceDetails.h"
#include "../../Voice/MasteringVoice/MasteringVoice.h"
#include "../../Voice/Output/OutputVoices.h"
#include "../../Volume/SoundVolume.h"
#include "../../../DebugLayer/Debug.h"
#include "../../../Device/Flag.h"

SourceVoice::SourceVoice(IXAudio2SourceVoice* XAudio2SourceVoice, MasteringVoice& masteringVoice, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format, const SourceVoiceInitParam& param) :
    mXAudio2SourceVoice(XAudio2SourceVoice),
    mDetails{ format.channels, format.samplesPerSec },
    mSoundData(std::make_unique<SoundData>(*loader, format)),
    mSoundPlayer(std::make_unique<SoundPlayer>(*this, loader, format, param.maxFrequencyRatio)),
    mSoundVolume(std::make_unique<SoundVolume>(*this, format.channels, masteringVoice.getVoiceDetails().channels)),
    mOutputVoices(std::make_unique<OutputVoices>(*this)),
    mSoundEffect(std::make_unique<SoundEffect>(*this, param.flags.check(SoundFlags::USE_FILTER))) {

    //再生時間設定/取得のためにすべてのソースボイスでエフェクト付与
    mSoundEffect->getEffectCollection().getPlayTimer();
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
    mSoundPlayer->update();
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
