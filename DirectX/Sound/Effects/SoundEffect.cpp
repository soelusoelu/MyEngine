#include "SoundEffect.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../Voice/MasteringVoice/OutputVoiceDetails.h"
#include "../../DebugLayer/Debug.h"

SoundEffect::SoundEffect(IVoice& voice, MasteringVoice& masteringVoice) :
    mVoice(voice),
    mMasteringVoice(masteringVoice),
    mDescs(),
    XAPO(nullptr)
{
}

SoundEffect::~SoundEffect() = default;

void SoundEffect::reverb(bool initialState) {
    auto res = XAudio2CreateReverb(&XAPO);
    if (FAILED(res)) {
        Debug::logError("Failed created reverb.");
        return;
    }

    XAUDIO2_EFFECT_DESCRIPTOR desc;
    desc.InitialState = initialState;
    desc.OutputChannels = mMasteringVoice.getDetails().outputChannels;
    desc.pEffect = XAPO;

    mDescs.emplace_back(desc);
}

void SoundEffect::set() {
    XAUDIO2_EFFECT_CHAIN chain;
    chain.EffectCount = mDescs.size();
    chain.pEffectDescriptors = mDescs.data();

    auto res = mVoice.getXAudio2Voice()->SetEffectChain(&chain);

    auto ref = XAPO->Release();
    XAPO = nullptr;

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed set effect chain.");
    }
#endif // _DEBUG
}
