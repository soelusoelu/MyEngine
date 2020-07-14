#include "Reverb.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../Voice/MasteringVoice/OutputVoiceDetails.h"
#include "../../DebugLayer/Debug.h"

Reverb::Reverb(IVoice& voice, MasteringVoice& masteringVoice) :
    SoundEffectBase(voice, masteringVoice) {
}

Reverb::~Reverb() = default;

bool Reverb::create(XAUDIO2_EFFECT_DESCRIPTOR* desc, bool initialState) {
    auto res = XAudio2CreateReverb(&mXAPO);
    if (FAILED(res)) {
        Debug::logError("Failed created reverb.");
        return false;
    }

    desc->InitialState = initialState;
    desc->OutputChannels = mMasteringVoice.getDetails().outputChannels;
    desc->pEffect = mXAPO.Get();

    return true;
}
