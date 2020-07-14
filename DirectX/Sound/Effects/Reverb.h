#pragma once

#include "SoundEffectBase.h"
#include "../Voice/IVoice.h"
#include <xaudio2.h>
#include <xaudio2fx.h>

#pragma comment(lib, "XAudio2.lib")

class MasteringVoice;

//リバーブ
class Reverb : public SoundEffectBase {
public:
    Reverb(IVoice& voice, MasteringVoice& masteringVoice);
    ~Reverb();
     virtual bool create(XAUDIO2_EFFECT_DESCRIPTOR* desc, bool initialState = true) override;

private:
    Reverb(const Reverb&) = delete;
    Reverb& operator=(const Reverb&) = delete;
};
