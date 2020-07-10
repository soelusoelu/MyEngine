#pragma once

#include "../System/GlobalFunction.h"
#include <xaudio2.h>

//波形データ構造体
struct WaveformData {
    //波形データ(フォーマット等を含まない、純粋に波形データのみ)
    BYTE* buffer;
    //波形データのサイズ
    unsigned size;
    //すべての非PCMフォーマットに使用される拡張波形フォーマット構造
    WAVEFORMATEX* format;

    WaveformData() :
        buffer(nullptr),
        size(0),
        format(nullptr) {
    }

    ~WaveformData() {
        safeDelete(buffer);
        safeDelete(format);
    }
};
