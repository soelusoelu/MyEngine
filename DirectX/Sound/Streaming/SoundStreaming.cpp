#include "SoundStreaming.h"
#include "../Data/SoundData.h"
#include "../Player/SoundPlayer.h"
#include "../Player/SoundPlayTimer.h"
#include "../Player/Buffer/BufferSubmitter.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"
#include "../../System/GlobalFunction.h"
#include <utility>

SoundStreaming::SoundStreaming(SourceVoice& sourceVoice, SoundPlayer& player, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format) :
    mSourceVoice(sourceVoice),
    mPlayer(player),
    mBufferSubmitter(std::make_unique<BufferSubmitter>(sourceVoice)),
    mLoader(std::move(loader)),
    mBuffer{ nullptr, nullptr },
    READ_SIZE(format.avgBytesPerSec * SEC),
    mRemainBufferSize(0),
    mWrite(0),
    mEndOfFile(false) {
    for (size_t i = 0; i < BUFFER_COUNT; i++) {
        mBuffer[i] = new BYTE[READ_SIZE];
    }
    mRemainBufferSize = mLoader->size() % READ_SIZE;
}

SoundStreaming::~SoundStreaming() {
    for (size_t i = 0; i < BUFFER_COUNT; i++) {
        safeDeleteArray(mBuffer[i]);
    }
}

void SoundStreaming::polling() {
    //ファイルの終端ならこれ以上読み込めない
    if (mEndOfFile) {
        return;
    }

    XAUDIO2_VOICE_STATE state = { 0 };
    mSourceVoice.getXAudio2SourceVoice()->GetState(&state);
    //再生キューがBUFFER_SIZE未満なら新たにバッファを追加する
    if (state.BuffersQueued < BUFFER_COUNT) {
        addBuffer();
    }
}

void SoundStreaming::seek(float point) {
    initialize();

    //シーク位置が曲の最初を指しているならここで終わる
    if (Math::nearZero(point)) {
        return;
    }

    const auto& data = mSourceVoice.getSoundData();
    data.clamp(point);
    auto seekPoint = static_cast<unsigned>(data.averageBytesPerSec * point);

    recomputeRemainBufferSize(seekPoint);

    auto res = mLoader->seek(static_cast<int>(seekPoint));
    //0より大きければ正しくシークできてる
    if (res > 0) {
        mWrite = res;
    } else {
        Debug::logWarning("Failed streaming seek.");
    }
}

void SoundStreaming::addBuffer() {
    std::swap(mBuffer[PRIMARY], mBuffer[SECONDARY]);

    long res = 0;
    //次の読み込みがデータサイズを超えるなら
    if (mWrite + READ_SIZE > mLoader->size()) {
        res = read(mRemainBufferSize);
        mEndOfFile = true;
    } else {
        //再生時間を揃えるために無理やり
        if (mWrite == 0) {
            mPlayer.getPlayTimer().setPlayTime(0.f);
        }

        res = read(READ_SIZE);
        mWrite += res;
    }

    //バッファ作成
    SimpleSoundBuffer buf;
    buf.buffer = mBuffer[SECONDARY];
    buf.size = res;

    mBufferSubmitter->submitSimpleBuffer(buf);
}

long SoundStreaming::read(unsigned readSize) {
    return mLoader->read(&mBuffer[SECONDARY], readSize);
}

void SoundStreaming::initialize() {
    mLoader->seekBegin();
    mWrite = 0;
    mEndOfFile = false;
    recomputeRemainBufferSize(0);
}

void SoundStreaming::recomputeRemainBufferSize(unsigned point) {
    auto sub = mLoader->size() - point;
    mRemainBufferSize = sub % READ_SIZE;
}
