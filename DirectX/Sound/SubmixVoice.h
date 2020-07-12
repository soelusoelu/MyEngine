#pragma once

#include "IVoice.h"
#include "SubmixVoiceInitParam.h"
#include <xaudio2.h>
#include <memory>

class MasteringVoice;
class SoundEffect;

//IXAudio2SubmixVoiceラッパークラス
class SubmixVoice : public IVoice {
public:
    SubmixVoice(IXAudio2SubmixVoice* XAudio2SubmixVoice, MasteringVoice& masteringVoice, const SubmixVoiceInitParam& param);
    ~SubmixVoice();

    virtual IXAudio2Voice* getXAudio2Voice() const override;

    /// <summary>
    /// サウンドエフェクト設定クラスを返す
    /// </summary>
    /// <returns></returns>
    SoundEffect& getSoundEffect() const;

private:
    SubmixVoice(const SubmixVoice&) = delete;
    SubmixVoice& operator=(const SubmixVoice&) = delete;

private:
    IXAudio2SubmixVoice* mXAudio2SubmixVoice;
    MasteringVoice& mMasteringVoice;
    std::unique_ptr<SoundEffect> mSoundEffect;
};
