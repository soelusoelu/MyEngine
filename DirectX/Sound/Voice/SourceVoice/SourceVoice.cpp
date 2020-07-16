#include "SourceVoice.h"
#include "../SubmixVoice/SubmixVoice.h"
#include "../../Effects/SoundFilter.h"
#include "../../Flag/SoundFlag.h"
#include "../../Output/OutputVoices.h"
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
    mSoundVolume(nullptr),
    mOutputVoices(std::make_unique<OutputVoices>(*this)),
    mSoundFilter(std::make_unique<SoundFilter>(*this, param.flags.check(static_cast<unsigned>(SoundFlag::USE_FILTER)))) {

    mDetails.inputChannels = data.format->nChannels;
    mDetails.samplesPerSec = data.format->nSamplesPerSec;
    mSoundBuffer->buffer = data.buffer;
    mSoundBuffer->size = data.size;

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

void SourceVoice::update() {
    mOutputVoices->update();
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
