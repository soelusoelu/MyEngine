#pragma once

#include "../System/SystemInclude.h"
#include <xaudio2.h>

//波形データの構造体
struct SoundBuffer {
    //XAUDIO2_END_OF_STREAMを入れるとバッファの後続がないことを示す
    //再生後、ボイスは自動的に終了停止状態になる
    //それ以外では0
    unsigned flags;
    //波形データ全体のサイズ(単位: バイト)
    //XAUDIO2_MAX_BUFFER_BYTES(2GB)が上限
    unsigned size;
    //ヘッダ等を含まない波形データ本体の先頭アドレス
    const BYTE* buffer;
    //再生の開始位置(単位: 秒)
    //これより前のデータは無視される
    //0だと頭から再生
    float playBegin;
    //再生領域の長さ(単位: 秒)
    //playBegin + playLengthより後ろのデータは無視される
    //0だと全体再生
    float playLength;
    //ループの開始位置(単位: 秒)
    //ループして折り返す際に戻る位置
    float loopBegin;
    //ループの長さ(単位: 秒)
    //loopBegin + loopLengthが実際の折り返し位置
    float loopLength;
    //ループする回数
    //0でループなし、XAUDIO2_LOOP_INFINITEで永久ループ
    unsigned loopCount;
    //コールバックで返されるコンテキスト値
    void* context;

    SoundBuffer() :
        flags(XAUDIO2_END_OF_STREAM),
        size(0),
        buffer(0),
        playBegin(0.f),
        playLength(0.f),
        loopBegin(0.f),
        loopLength(0.f),
        loopCount(0),
        context(nullptr) {
    }
};
