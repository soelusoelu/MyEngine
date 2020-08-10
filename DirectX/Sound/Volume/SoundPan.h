#pragma once

#include "../Voice/IVoice.h"
#include <xaudio2.h>

class MasteringVoice;

//音の定位を扱うクラス
class SoundPan {
public:
    SoundPan(IVoice& voice, const MasteringVoice& masteringVoice);
    ~SoundPan();

    /// <summary>
    /// 位置によって音をパンする
    /// </summary>
    /// <param name="positionX">音を鳴らす位置(0～1920)</param>
    /// <param name="operationSet">いつ実行するか</param>
    void pan(float positionX, unsigned operationSet = XAUDIO2_COMMIT_NOW);

    /// <summary>
    /// 音の定位を中心に戻す
    /// </summary>
    /// <param name="operationSet"></param>
    void panCenter(unsigned operationSet = XAUDIO2_COMMIT_NOW);

private:
    SoundPan(const SoundPan&) = delete;
    SoundPan& operator=(const SoundPan&) = delete;

    //出力先を決定する
    void selectOutput(const float volumes[], unsigned operationSet);
    //パンを実行する
    void setOutputMatrix(IXAudio2Voice* outputVoice, const float volumes[], unsigned operationSet);

private:
    IVoice& mVoice;
    const unsigned INPUT_CHANNELS;
    const unsigned OUTPUT_CHANNELS;
};
