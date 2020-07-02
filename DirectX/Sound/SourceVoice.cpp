#include "SourceVoice.h"
#include "../DebugLayer/Debug.h"

SourceVoice::SourceVoice() :
    mSourceVoice(nullptr) {
}

SourceVoice::~SourceVoice() {
    mSourceVoice->DestroyVoice();
    mSourceVoice = nullptr;
}

void SourceVoice::play(unsigned flags, unsigned operationSet) const {
    auto res = mSourceVoice->Start(flags, operationSet);
    if (FAILED(res)) {
        Debug::logError("Failed sound play.");
    }
}

void SourceVoice::stop(unsigned flags, unsigned operationSet) const {
    auto res = mSourceVoice->Stop(flags, operationSet);
    if (FAILED(res)) {
        Debug::logError("Failed suond stop.");
    }
}

void SourceVoice::submitSourceBuffer(const SoundBuffer& buffer) const {
    auto res = mSourceVoice->SubmitSourceBuffer(&toBuffer(buffer), nullptr);
    if (FAILED(res)) {
        Debug::logError("Failed submit source buffer.");
    }
}

void SourceVoice::setVolume(float volume, unsigned operationSet) const {
    auto res = mSourceVoice->SetVolume(volume, operationSet);
    if (FAILED(res)) {
        Debug::logError("Failed volume setting.");
    }
}

XAUDIO2_BUFFER SourceVoice::toBuffer(const SoundBuffer& buffer) const {
    XAUDIO2_BUFFER buf;
    buf.Flags = buffer.flags;
    buf.AudioBytes = buffer.size;
    buf.pAudioData = buffer.buffer;
    buf.PlayBegin = buffer.playBegin;
    buf.PlayLength = buffer.playLength;
    buf.LoopBegin = buffer.loopBegin;
    buf.LoopLength = buffer.loopLength;
    buf.LoopCount = buffer.loopCount;
    buf.pContext = buffer.context;

    return buf;
}
