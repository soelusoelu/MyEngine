#include "SoundEffect.h"
#include "Reverb.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../Voice/MasteringVoice/OutputVoiceDetails.h"
#include "../../DebugLayer/Debug.h"
#include <memory>

SoundEffect::SoundEffect(IVoice& voice, MasteringVoice& masteringVoice) :
    mVoice(voice),
    mMasteringVoice(masteringVoice),
    mDescs() {
}

SoundEffect::~SoundEffect() = default;

int SoundEffect::reverb(bool initialState) {
    auto reverb = std::make_unique<Reverb>(mVoice, mMasteringVoice);
    XAUDIO2_EFFECT_DESCRIPTOR desc;
    bool res = reverb->create(&desc, initialState);
    if (!res) { //エフェクトの作成に失敗していたら-1
        return -1;
    }

    mDescs.emplace_back(desc);
    return mDescs.size() - 1;
}

void SoundEffect::apply() {
    XAUDIO2_EFFECT_CHAIN chain;
    chain.EffectCount = mDescs.size();
    chain.pEffectDescriptors = mDescs.data();

    auto res = mVoice.getXAudio2Voice()->SetEffectChain(&chain);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed set effect chain.");
    }
#endif // _DEBUG
}

void SoundEffect::setEffectParameters(int effectID, const void* parameters, unsigned parametersByteSize, unsigned operationSet) {
    //IDが有効か
    if (effectID == -1) {
        Debug::logWarning("Invalid Effect ID.");
        return;
    }

    auto res = mVoice.getXAudio2Voice()->SetEffectParameters(effectID, parameters, parametersByteSize, operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed set effect parameters.");
    }
#endif // _DEBUG
}
