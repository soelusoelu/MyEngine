#pragma once

#include "../Voice/IVoice.h"
#include <xaudio2.h>

class MasteringVoice;
class OutputVoices;

class SoundPan {
public:
    SoundPan(IVoice& voice, MasteringVoice& masteringVoice, OutputVoices& outputVoices);
    ~SoundPan();

    /// <summary>
    /// 位置によって音をパンする
    /// </summary>
    /// <param name="positionX">音を鳴らす位置(0～1920)</param>
    /// <param name="operationSet">いつ実行するか</param>
    void pan(float positionX, unsigned operationSet = XAUDIO2_COMMIT_NOW);

private:
    SoundPan(const SoundPan&) = delete;
    SoundPan& operator=(const SoundPan&) = delete;

    //パンを実行する
    void setOutputMatrix(IXAudio2Voice* outputVoice, const float volumes[], unsigned operationSet);

private:
    IVoice& mVoice;
    OutputVoices& mOutputVoices;
    const unsigned INPUT_CHANNELS;
    const unsigned OUTPUT_CHANNELS;
};
