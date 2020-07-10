#pragma once

#include "SubmixVoiceInitParam.h"
#include <xaudio2.h>

//IXAudio2SubmixVoiceラッパークラス
class SubmixVoice {
public:
    SubmixVoice(IXAudio2SubmixVoice* XAudio2SubmixVoice, const SubmixVoiceInitParam& param);
    ~SubmixVoice();

    /// <summary>
    /// IXAudio2SubmixVoiceを返す
    /// </summary>
    /// <returns></returns>
    IXAudio2SubmixVoice* getXAudio2SubmixVoice() const;

private:
    SubmixVoice(const SubmixVoice&) = delete;
    SubmixVoice& operator=(const SubmixVoice&) = delete;

    IXAudio2SubmixVoice* mXAudio2SubmixVoice;
};
