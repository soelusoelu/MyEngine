#pragma once

#include "../Loader/ISoundLoader.h"
#include "../Loader/WaveFormat.h"
#include <memory>

class SourceVoice;
class BufferSubmitter;

//ストリーミングを扱うクラス
class SoundStreaming {
public:
    SoundStreaming(SourceVoice& sourceVoice, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format);
    ~SoundStreaming();

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// 再生を開始する
    /// </summary>
    void play();

    /// <summary>
    /// 指定したファイル位置までシークする
    /// </summary>
    /// <param name="point">ファイルの先頭からの位置(単位: 秒数)</param>
    void seek(float point);

    /// <summary>
    /// 次の読み込み後の位置をバイト単位で返す
    /// </summary>
    /// <returns></returns>
    unsigned getNextReadPointInByte() const;

private:
    SoundStreaming(const SoundStreaming&) = delete;
    SoundStreaming& operator=(const SoundStreaming&) = delete;

    //バッファの状態を監視する
    void polling();
    //バッファを追加する
    void addBuffer();
    //ファイルの先頭までシークする
    void seekBegin();

private:
    static constexpr unsigned BUFFER_COUNT = 3;
    static constexpr unsigned PRIMARY = 0;
    static constexpr unsigned SECONDARY = 1;
    static constexpr unsigned REMAIN = 2;
    static constexpr float SEC = 1.f;

    SourceVoice& mSourceVoice;
    std::unique_ptr<BufferSubmitter> mBufferSubmitter;
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
