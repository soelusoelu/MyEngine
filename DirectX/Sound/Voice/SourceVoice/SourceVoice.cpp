#include "SourceVoice.h"
#include "VoiceDetails.h"
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
    mData(std::make_unique<VoiceDetails>(data)),
    mSoundPlayer(std::make_unique<SoundPlayer>(*this, param.maxFrequencyRatio)),
    mSoundVolume(std::make_unique<SoundVolume>(*this, masteringVoice)),
    mSoundFilter(std::make_unique<SoundFilter>(*this, param.flags.check(static_cast<unsigned>(SoundFlag::USE_FILTER)))) {

    mSoundBuffer->buffer = mData->buffer();
    mSoundBuffer->size = mData->size();
}

SourceVoice::~SourceVoice() {
    mXAudio2SourceVoice->DestroyVoice();
    mXAudio2SourceVoice = nullptr;
}

IXAudio2Voice* SourceVoice::getXAudio2Voice() const {
    return mXAudio2SourceVoice;
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

void SourceVoice::setOutputVoice(const IVoice& voice, bool useFilter) {
    XAUDIO2_SEND_DESCRIPTOR desc;
    desc.Flags = (useFilter) ? XAUDIO2_SEND_USEFILTER : 0;
    desc.pOutputVoice = voice.getXAudio2Voice();
    XAUDIO2_VOICE_SENDS sends;
    sends.SendCount = 1;
    sends.pSends = &desc;
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

VoiceDetails& SourceVoice::getSoundData() const {
    return *mData;
}

SoundPlayer& SourceVoice::getSoundPlayer() const {
    return *mSoundPlayer;
}

SoundVolume& SourceVoice::getSoundVolume() const {
    return *mSoundVolume;
}

SoundFilter& SourceVoice::getSoundFilter() const {
    return *mSoundFilter;
}

XAUDIO2_BUFFER SourceVoice::toBuffer(const SoundBuffer& buffer) const {
    unsigned sampleRate = mData->getSampleRate();

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
