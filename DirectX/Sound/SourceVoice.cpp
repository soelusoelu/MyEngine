#include "SourceVoice.h"
#include "SoundEffect.h"
#include "SoundLoader.h"
#include "SoundPlayer.h"
#include "SoundVolume.h"
#include "VoiceDetails.h"
#include "../DebugLayer/Debug.h"

SourceVoice::SourceVoice(IXAudio2SourceVoice* XAudio2SourceVoice, const SoundLoader& data) :
    mXAudio2SourceVoice(XAudio2SourceVoice),
    mSoundBuffer(std::make_unique<SoundBuffer>()),
    mData(std::make_unique<VoiceDetails>(data)),
    mSoundPlayer(std::make_unique<SoundPlayer>(*this)),
    mSoundVolume(std::make_unique<SoundVolume>(*this)),
    mSoundEffect(std::make_unique<SoundEffect>(*this)) {

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

SoundBuffer& SourceVoice::getSoundBuffer() const {
    return *mSoundBuffer;
}

VoiceDetails& SourceVoice::getSoundData() const {
    return *mData;
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

SoundPlayer& SourceVoice::getSoundPlayer() const {
    return *mSoundPlayer;
}

SoundVolume& SourceVoice::getSoundVolume() const {
    return *mSoundVolume;
}

SoundEffect& SourceVoice::getSoundEffect() const {
    return *mSoundEffect;
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
