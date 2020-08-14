#include "DspSetter.h"
#include "../Effects/SoundEffect.h"
#include "../Effects/SoundEffectCollection.h"
#include "../Effects/Filter/SoundFilter.h"
#include "../Player/Frequency.h"
#include "../Player/SoundPlayer.h"
#include "../Voice/VoiceDetails.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../Volume/SoundPan.h"
#include "../Volume/SoundVolume.h"
#include "../../DebugLayer/Debug.h"

DspSetter::DspSetter(SourceVoice& sourceVoice, const MasteringVoice& masteringVoice, const WaveFormat& format) :
    mSourceVoice(sourceVoice),
    mMasteringVoice(masteringVoice),
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

void DspSetter::applyToSourceVoice(const Flag& flag) {
    //3D計算(絶対設定されているはず)
    if (flag.check(X3DAUDIO_CALCULATE_MATRIX)) {
        mSourceVoice.getSoundVolume().getPan().pan(mMatrixCoefficients.data());
    } else {
        Debug::logError("X3DAUDIO_CALCULATE_MATRIX is not flagged.");
    }
    //ドップラー
    if (flag.check(X3DAUDIO_CALCULATE_DOPPLER)) {
        mSourceVoice.getSoundPlayer().getFrequency().setFrequencyRatio(mDspSettings->DopplerFactor);
    }
    //ローパスフィルタ直接パス
    if (flag.check(X3DAUDIO_CALCULATE_LPF_DIRECT)) {
        //XAUDIO2_FILTER_PARAMETERS param = { XAUDIO2_FILTER_TYPE::LowPassFilter, mDspSettings->LPFDirectCoefficient, 1.4142135f };
        //mSourceVoice.getXAudio2Voice()->SetOutputFilterParameters(mMasteringVoice.getXAudio2Voice(), &param);
        mSourceVoice.getSoundEffect().getEffectCollection().getFilter().lowPassFilter(mDspSettings->LPFDirectCoefficient * mSourceVoice.getVoiceDetails().sampleRate / 6.f);
    }
    //ローパスフィルタリバーブパス
    if (flag.check(X3DAUDIO_CALCULATE_LPF_REVERB)) {
    }
    //リバーブ
    if (flag.check(X3DAUDIO_CALCULATE_REVERB)) {
    }
}

X3DAUDIO_DSP_SETTINGS& DspSetter::getDspSetting() const {
    return *mDspSettings;
}
