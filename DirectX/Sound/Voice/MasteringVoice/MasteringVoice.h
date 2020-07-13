#pragma once

#include "OutputVoiceDetails.h"
#include <xaudio2.h>

//IXAudio2MasteringVoiceラッパークラス
class MasteringVoice {
public:
    MasteringVoice(IXAudio2MasteringVoice* XAudio2MasteringVoice);
    ~MasteringVoice();

    /// <summary>
    /// アウトプット用サウンドデータを返す
    /// </summary>
    /// <returns></returns>
    const OutputVoiceDetails& getDetails() const;

private:
    MasteringVoice(const MasteringVoice&) = delete;
    MasteringVoice& operator=(const MasteringVoice&) = delete;

    //XAUDIO2_VOICE_DETAILSからOutputVoiceDetailsへの変換
    void toOutputVoiceDetails(const XAUDIO2_VOICE_DETAILS& details);

private:
    IXAudio2MasteringVoice* mXAudio2MasteringVoice;
    OutputVoiceDetails mDetails;
};
