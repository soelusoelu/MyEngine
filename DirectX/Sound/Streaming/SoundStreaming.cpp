#include "SoundStreaming.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../../DebugLayer/Debug.h"
#include "../../System/GlobalFunction.h"
#include <utility>

SoundStreaming::SoundStreaming(SourceVoice& sourceVoice, IBufferSubmitter& bufferSubmitter, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format) :
    mSourceVoice(sourceVoice),
    mBufferSubmitter(bufferSubmitter),
    mLoader(std::move(loader)),
    mBuffer{ new BYTE[format.avgBytesPerSec * SEC], new BYTE[format.avgBytesPerSec * SEC] },
    mIsPlayStreaming(false),
    mWrite(0),
    READ_SIZE(static_cast<unsigned>(format.avgBytesPerSec * SEC)) {
}

SoundStreaming::~SoundStreaming() {
    for (size_t i = 0; i < BUFFER_SIZE; i++) {
        safeDelete(mBuffer[i]);
    }
}

void SoundStreaming::play() {
    mIsPlayStreaming = true;
    polling();
}

void SoundStreaming::update() {
    if (mIsPlayStreaming) {
        polling();
    }
}

void SoundStreaming::polling() {
    XAUDIO2_VOICE_STATE state;
    mSourceVoice.getXAudio2SourceVoice()->GetState(&state);
    //再生キューがBUFFER_SIZE未満なら新たにバッファを追加する
    if (state.BuffersQueued < BUFFER_SIZE) {
        addBuffer();
    }
}

void SoundStreaming::addBuffer() {
    std::swap(mBuffer[PRIMARY], mBuffer[SECONDARY]);
    auto res = mLoader->read(&mBuffer[SECONDARY], READ_SIZE);
    if (res <= 0) {
        //mIsPlayStreaming = false;
        mLoader->seekBegin();
        Debug::logWarning("Streaming playback failed to read.");
        return;
    }

    mWrite += res;

    //バッファ作成
    SoundBuffer buf;
    buf.buffer = mBuffer[SECONDARY];
    buf.size = res;

    mBufferSubmitter.submitSourceBuffer(buf);
}
