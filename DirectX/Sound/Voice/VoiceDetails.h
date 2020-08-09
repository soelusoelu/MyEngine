#pragma once

#include "../Loader/ISoundLoader.h"
#include "../Loader/WaveFormat.h"
#include "../../System/SystemInclude.h"

//サウンドデータ保存クラス
struct SoundData {
    //波形データのサイズ
    const unsigned size;
    //ビットレート
    const unsigned bitRate;
    //1サンプルのバイト数
    const unsigned blockAlign;
    //1秒間のバイト数
    const unsigned averageBytesPerSec;

    SoundData(const ISoundLoader& loader, const WaveFormat& format) :
        size(loader.size()),
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
