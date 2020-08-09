#include "SoundStreaming.h"
#include "../Player/BufferSubmitter.h"
#include "../Voice/VoiceDetails.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../../DebugLayer/Debug.h"
#include "../../System/GlobalFunction.h"
#include <utility>

SoundStreaming::SoundStreaming(SourceVoice& sourceVoice, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format) :
    mSourceVoice(sourceVoice),
    mBufferSubmitter(std::make_unique<BufferSubmitter>(sourceVoice)),
    mLoader(std::move(loader)),
    mBuffer{ nullptr, nullptr, nullptr },
    READ_SIZE(format.avgBytesPerSec* SEC),
    mWrite(0),
    mRemainBufferSize(0),
    mIsPlayStreaming(false)
{
    for (size_t i = 0; i < BUFFER_COUNT - 1; i++) {
        mBuffer[i] = new BYTE[READ_SIZE];
    }
    mRemainBufferSize = mLoader->size() % READ_SIZE;
    mBuffer[REMAIN] = new BYTE[mRemainBufferSize];
}

SoundStreaming::~SoundStreaming() {
    for (size_t i = 0; i < BUFFER_COUNT; i++) {
        safeDeleteArray(mBuffer[i]);
    }
}

void SoundStreaming::update() {
    if (mIsPlayStreaming) {
        polling();
    }
}

void SoundStreaming::play() {
    mIsPlayStreaming = true;
    polling();
}

void SoundStreaming::seek(float point) {
    mLoader->seekBegin();
    auto seekPoint = mSourceVoice.getSoundData().averageBytesPerSec * point;
    auto res = mLoader->seek(static_cast<int>(seekPoint));
    //0より大きければ正しくシークできてる
    if (res > 0) {
        mWrite = res;
    } else {
        Debug::logWarning("Failed streaming seek.");
    }
}

unsigned SoundStreaming::getNextReadPointInByte() const {
    return mWrite + READ_SIZE;
}

void SoundStreaming::polling() {
    XAUDIO2_VOICE_STATE state;
    mSourceVoice.getXAudio2SourceVoice()->GetState(&state);
    //再生キューがBUFFER_SIZE未満なら新たにバッファを追加する
    if (state.BuffersQueued < 2) {
        addBuffer();
    }
}

void SoundStreaming::addBuffer() {
    std::swap(mBuffer[PRIMARY], mBuffer[SECONDARY]);

    SimpleSoundBuffer buf;
    auto res = 0;
    if (mWrite + READ_SIZE > mLoader->size()) {
        res = mLoader->read(&mBuffer[REMAIN], mRemainBufferSize);
        buf.buffer = mBuffer[REMAIN];
        seekBegin();
    } else {
        res = mLoader->read(&mBuffer[SECONDARY], READ_SIZE);
        buf.buffer = mBuffer[SECONDARY];
    }

    mWrite += res;

    //バッファ作成
    //SoundBuffer buf;
    //buf.buffer = mBuffer[SECONDARY];
    buf.size = res;

    mBufferSubmitter->submitSimpleBuffer(buf);
}

void SoundStreaming::seekBegin() {
    mLoader->seekBegin();
    mWrite = 0;
}
