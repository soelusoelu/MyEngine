#pragma once

#include "ISoundLoader.h"

#pragma comment(lib, "winmm.lib")

class WAV : public ISoundLoader {
    enum class FindFlag {
        NONE = 0,
        CHUNK = MMIO_FINDCHUNK, //指定された識別子を持つチャンクを検索
        RIFF = MMIO_FINDRIFF, //識別子「RIFF」と指定されたチャンクを検索
        LIST = MMIO_FINDLIST, //識別子「LIST」と指定されたチャンクを検索
    };

public:
    WAV();
    ~WAV();
    virtual std::shared_ptr<WaveformData> loadFromFile(const std::string& fileName) override;

private:
    //WAVファイルを開く
    void open(const std::string& fileName);
    //チャンク全体を読み込む
    long read(void* out, long size) const;
    //チャンクを読み込む
    bool descend(MMCKINFO* out, const MMCKINFO* parent, FindFlag flag) const;
    bool ascend(MMCKINFO* out);
    //チャンクを設定する
    void setChunkID(const char* ch);
    //FourCCを取得する
    FOURCC getFourCC(const char* ch) const;
    //WAVファイルか
    bool isWavFile(const MMCKINFO& riffChunk) const;
    //Waveフォーマットを作成する
    void createWaveFormat(WAVEFORMATEX* dst, const PCMWAVEFORMAT& src);

private:
    //WindowsマルチメディアAPIのハンドル(WindowsマルチメディアAPIはWAVファイル関係の操作用のAPI)
    HMMIO mHMmio;
    //チャンク情報
    MMCKINFO mChunkInfo;
    //最上部チャンク(RIFFチャンク)保存用
    MMCKINFO mRiffChunkInfo;

    //チャンクIDのサイズ
    static constexpr unsigned char CHUNK_ID_SIZE = 4;
    //チャンクの種類
    static constexpr const char CHUNK_FORMAT[CHUNK_ID_SIZE] = { 'f', 'm', 't', ' ' };
    static constexpr const char CHUNK_DATA[CHUNK_ID_SIZE] = { 'd', 'a', 't', 'a' };
    static constexpr const char CHUNK_RIFF[CHUNK_ID_SIZE] = { 'R', 'I', 'F', 'F' };
    static constexpr const char CHUNK_WAVE[CHUNK_ID_SIZE] = { 'W', 'A', 'V', 'E' };
};
