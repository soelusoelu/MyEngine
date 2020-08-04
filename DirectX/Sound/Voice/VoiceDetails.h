#pragma once

#include "../Loader/WaveformData.h"
#include "../../System/SystemInclude.h"

//サウンドデータ保存クラス
struct SoundData {
    //ヘッダ等を含まない純粋な波形データ
    const BYTE* buffer;
    //波形データのサイズ
    const unsigned size;
    //ビットレート
    const unsigned bitRate;
    //1サンプルのバイト数
    const unsigned blockAlign;
    //1秒間のバイト数
    const unsigned averageBytesPerSec;

    SoundData(const WaveformData& data) :
        buffer(data.buffer),
        size(data.size),
        bitRate(data.format->wBitsPerSample),
        blockAlign(data.format->nBlockAlign),
        averageBytesPerSec(data.format->nAvgBytesPerSec) {
    }
};

//ボイス詳細保存クラス
struct VoiceDetails {
    //入力チャンネル数
    unsigned channels;
    //サンプリングレート
    unsigned sampleRate;

    VoiceDetails() :
        channels(0),
        sampleRate(0) {
    }
};
