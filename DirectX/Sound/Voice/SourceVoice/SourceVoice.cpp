#include "SourceVoice.h"
#include "../SubmixVoice/SubmixVoice.h"
#include "../../Effects/SoundFilter.h"
#include "../../Flag/SoundFlag.h"
#include "../../Player/SoundPlayer.h"
#include "../../Volume/SoundVolume.h"
#include "../../../DebugLayer/Debug.h"
#include "../../../Device/Flag.h"

SourceVoice::SourceVoice(IXAudio2SourceVoice* XAudio2SourceVoice, MasteringVoice& masteringVoice, const WaveformData& data, const SourceVoiceInitParam& param) :
    mXAudio2SourceVoice(XAudio2SourceVoice),
    mMasteringVoice(masteringVoice),
    mSoundBuffer(std::make_unique<SoundBuffer>()),
    mDetails(),
    mSoundData(std::make_unique<SoundData>(data)),
    mSoundPlayer(std::make_unique<SoundPlayer>(*this, param.maxFrequencyRatio)),
    mSoundVolume(std::make_unique<SoundVolume>(*this, masteringVoice)),
    mSoundFilter(std::make_unique<SoundFilter>(*this, param.flags.check(static_cast<unsigned>(SoundFlag::USE_FILTER)))) {

    mDetails.inputChannels = data.format->nChannels;
    mDetails.samplesPerSec = data.format->nSamplesPerSec;
    mSoundBuffer->buffer = data.buffer;
    mSoundBuffer->size = data.size;
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

void SourceVoice::update() {
    mSoundVolume->update();
}

IXAudio2SourceVoice* SourceVoice::getXAudio2SourceVoice() const {
    return mXAudio2SourceVoice;
}

void SourceVoice::submitSourceBuffer(const SoundBuffer& buffer) const {
    auto res = mXAudio2SourceVoice->SubmitSourceBuffer(&toBuffer(buffer), nullptr);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed submit source buffer.");
    }
#endif // _DEBUG
}

void SourceVoice::submitSourceBuffer() const {
    submitSourceBuffer(*mSoundBuffer);
}

void SourceVoice::setOutputVoices(const std::vector<std::shared_ptr<IVoice>>& voices, bool useFilter) {
    if (voices.empty()) {
        Debug::logWarning("Output voices is empty.");
        return;
    }

    //設定するボイスの数に合わせてデスクリプタを作成する
    const size_t voiceSize = voices.size();
    std::vector<XAUDIO2_SEND_DESCRIPTOR> descs(voiceSize);
    for (size_t i = 0; i < voiceSize; i++) {
        descs[i].Flags = (useFilter) ? XAUDIO2_SEND_USEFILTER : 0;
        descs[i].pOutputVoice = voices[i]->getXAudio2Voice();
    }
    //実際に送るデータの設定
    XAUDIO2_VOICE_SENDS sends;
    sends.SendCount = voiceSize;
    sends.pSends = descs.data();
    auto res = mXAudio2SourceVoice->SetOutputVoices(&sends);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed set output voices.");
    }
#endif // _DEBUG
}

SoundBuffer& SourceVoice::getSoundBuffer() const {
    return *mSoundBuffer;
}

SoundData& SourceVoice::getSoundData() const {
    return *mSoundData;
}

SoundPlayer& SourceVoice::getSoundPlayer() const {
    return *mSoundPlayer;
}

SoundFilter& SourceVoice::getSoundFilter() const {
    return *mSoundFilter;
}

XAUDIO2_BUFFER SourceVoice::toBuffer(const SoundBuffer& buffer) const {
    const unsigned sampleRate = mDetails.samplesPerSec;

    XAUDIO2_BUFFER buf;
    buf.Flags = buffer.flags;
    buf.AudioBytes = buffer.size;
    buf.pAudioData = buffer.buffer;
    buf.PlayBegin = static_cast<unsigned>(buffer.playBegin * sampleRate);
    buf.PlayLength = static_cast<unsigned>(buffer.playLength * sampleRate);
    buf.LoopBegin = static_cast<unsigned>(buffer.loopBegin * sampleRate);
    buf.LoopLength = static_cast<unsigned>(buffer.loopLength * sampleRate);
    buf.LoopCount = buffer.loopCount;
    buf.pContext = buffer.context;

    return buf;
}
