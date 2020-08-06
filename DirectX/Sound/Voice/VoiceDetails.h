#pragma once

#include "../Loader/WaveFormat.h"
#include "../../System/SystemInclude.h"

//サウンドデータ保存クラス
struct SoundData {
    //ビットレート
    const unsigned bitRate;
    //1サンプルのバイト数
    const unsigned blockAlign;
    //1秒間のバイト数
    const unsigned averageBytesPerSec;

    SoundData(const WaveFormat& format) :
        bitRate(format.bitsPerSample),
        blockAlign(format.blockAlign),
        averageBytesPerSec(format.avgBytesPerSec) {
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
