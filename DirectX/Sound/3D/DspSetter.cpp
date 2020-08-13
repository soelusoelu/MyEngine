#include "DspSetter.h"
#include "../Player/Frequency.h"
#include "../Player/SoundPlayer.h"
#include "../Voice/VoiceDetails.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../Volume/SoundPan.h"
#include "../Volume/SoundVolume.h"

DspSetter::DspSetter(SourceVoice& sourceVoice, const MasteringVoice& masteringVoice, const WaveFormat& format) :
    mSourceVoice(sourceVoice),
    mDspSettings(std::make_unique<X3DAUDIO_DSP_SETTINGS>()) {
    const auto inCh = format.channels;
    const auto outCh = masteringVoice.getVoiceDetails().channels;

    mDspSettings->SrcChannelCount = inCh;
    mDspSettings->DstChannelCount = outCh;
    mMatrixCoefficients.resize(inCh * outCh);
    mDspSettings->pMatrixCoefficients = mMatrixCoefficients.data();
    mDspSettings->pDelayTimes = nullptr;
}

DspSetter::~DspSetter() = default;

void DspSetter::applyToSourceVoice() {
    mSourceVoice.getSoundPlayer().getFrequency().setFrequencyRatio(mDspSettings->DopplerFactor);
    mSourceVoice.getSoundVolume().getPan().pan(mMatrixCoefficients.data());
}

X3DAUDIO_DSP_SETTINGS& DspSetter::getDspSetting() const {
    return *mDspSettings;
}
