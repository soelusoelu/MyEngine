#pragma once

#include "../Loader/ISoundLoader.h"
#include "../Loader/WaveFormat.h"
#include "../Player/IBufferSubmitter.h"
#include <memory>

class SourceVoice;

class SoundStreaming {
public:
    SoundStreaming(SourceVoice& sourceVoice, IBufferSubmitter& bufferSubmitter, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format);
    ~SoundStreaming();
    void play();
    void update();

private:
    SoundStreaming(const SoundStreaming&) = delete;
    SoundStreaming& operator=(const SoundStreaming&) = delete;

    void polling();
    //バッファを追加する
    void addBuffer();

private:
    static constexpr unsigned BUFFER_COUNT = 3;
    static constexpr unsigned PRIMARY = 0;
    static constexpr unsigned SECONDARY = 1;
    static constexpr unsigned REMAIN = 2;
    static constexpr float SEC = 1.f;

    SourceVoice& mSourceVoice;
    IBufferSubmitter& mBufferSubmitter;
    std::unique_ptr<ISoundLoader> mLoader;
    //バッファ
    BYTE* mBuffer[BUFFER_COUNT];
    //ファイルから読み込むサイズ
    const unsigned READ_SIZE;
    //読み込みカーソル
    unsigned mWrite;
    //バッファの余り
    unsigned mRemainBufferSize;
    //ストリーミング再生中か
    bool mIsPlayStreaming;
};
