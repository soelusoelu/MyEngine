#pragma once

#include "../VoiceDetails.h"
#include <xaudio2.h>

//IXAudio2MasteringVoiceラッパークラス
class MasteringVoice {
public:
    MasteringVoice(IXAudio2MasteringVoice* XAudio2MasteringVoice);
    ~MasteringVoice();

    /// <summary>
    /// ボイスの詳細を返す
    /// </summary>
    /// <returns></returns>
    const VoiceDetails& getDetails() const;

private:
    MasteringVoice(const MasteringVoice&) = delete;
    MasteringVoice& operator=(const MasteringVoice&) = delete;

private:
    IXAudio2MasteringVoice* mXAudio2MasteringVoice;
    VoiceDetails mDetails;
};
