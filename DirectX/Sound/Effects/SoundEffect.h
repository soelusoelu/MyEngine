#pragma once

#include "../IVoice.h"
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <vector>

#pragma comment(lib, "XAudio2.lib")

class MasteringVoice;

class SoundEffect {
public:
    SoundEffect(IVoice& voice, MasteringVoice& masteringVoice);
    ~SoundEffect();

    /// <summary>
    /// リバーブ効果(残響)を掛ける
    /// </summary>
    /// <param name="initialState">初期状態でエフェクトを有効化するか</param>
    void reverb(bool initialState = true);

    /// <summary>
    /// エフェクトをボイスにセットする
    /// </summary>
    void set();

private:
    SoundEffect(const SoundEffect&) = delete;
    SoundEffect& operator=(const SoundEffect&) = delete;

private:
    IVoice& mVoice;
    MasteringVoice& mMasteringVoice;
    std::vector<XAUDIO2_EFFECT_DESCRIPTOR> mDescs;
    IUnknown* XAPO;
};
