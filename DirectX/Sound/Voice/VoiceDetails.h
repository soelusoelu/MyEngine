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
    const unsigned bitsPerSample;

    SoundData(const WaveformData& data) :
        buffer(data.buffer),
        size(data.size),
        bitsPerSample(data.format->wBitsPerSample) {
    }
};

//ボイス詳細保存クラス
struct VoiceDetails {
    //入力チャンネル数
    unsigned inputChannels;
    //サンプリングレート
    unsigned samplesPerSec;

    VoiceDetails() :
        inputChannels(0),
        samplesPerSec(0) {
    }
};
