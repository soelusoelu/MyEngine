#include "SourceVoice.h"
#include "SoundFilter.h"
#include "SoundFlag.h"
#include "SoundPlayer.h"
#include "SoundVolume.h"
#include "VoiceDetails.h"
#include "../DebugLayer/Debug.h"
#include "../Device/Flag.h"

SourceVoice::SourceVoice(IXAudio2SourceVoice* XAudio2SourceVoice, const WaveformData& data, const SourceVoiceInitParam& param) :
    mXAudio2SourceVoice(XAudio2SourceVoice),
    mSoundBuffer(std::make_unique<SoundBuffer>()),
    mData(std::make_unique<VoiceDetails>(data)),
    mSoundPlayer(std::make_unique<SoundPlayer>(*this, param.maxFrequencyRatio)),
    mSoundVolume(std::make_unique<SoundVolume>(*this)),
    mSoundFilter(std::make_unique<SoundFilter>(*this, param.flags.check(static_cast<unsigned>(SoundFlag::USE_FILTER)))) {

    mSoundBuffer->buffer = mData->buffer();
    mSoundBuffer->size = mData->size();
}

SourceVoice::~SourceVoice() {
    mXAudio2SourceVoice->DestroyVoice();
    mXAudio2SourceVoice = nullptr;
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

void SourceVoice::setOutputVoices(const XAUDIO2_VOICE_SENDS& sendlist) {
    mXAudio2SourceVoice->SetOutputVoices(&sendlist);
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
