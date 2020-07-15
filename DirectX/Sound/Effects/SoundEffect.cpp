#include "SoundEffect.h"
#include "Reverb.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../Voice/MasteringVoice/OutputVoiceDetails.h"
#include "../../DebugLayer/Debug.h"
#include <cassert>
#include <memory>

SoundEffect::SoundEffect(IVoice& voice, MasteringVoice& masteringVoice) :
    mVoice(voice),
    mMasteringVoice(masteringVoice),
    mDescs(),
    mIsApplied(false) {
}

SoundEffect::~SoundEffect() = default;

int SoundEffect::reverb(bool initialState) {
    auto reverb = std::make_unique<Reverb>();
    XAUDIO2_EFFECT_DESCRIPTOR desc;
    desc.InitialState = initialState;
    desc.OutputChannels = mMasteringVoice.getDetails().outputChannels;
    bool res = reverb->create(&desc);
    if (!res) { //エフェクトの作成に失敗していたら-1
        return -1;
    }

    mDescs.emplace_back(desc);
    return mDescs.size() - 1;
}

void SoundEffect::apply() {
    if (mDescs.empty()) {
        Debug::logWarning("Effect descriptor is empty.");
        return;
    }

    XAUDIO2_EFFECT_CHAIN chain;
    chain.EffectCount = mDescs.size();
    chain.pEffectDescriptors = mDescs.data();

    auto res = mVoice.getXAudio2Voice()->SetEffectChain(&chain);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed set effect chain.");
    }
#endif // _DEBUG

    mIsApplied = true;
}

void SoundEffect::setEffectParameters(int effectID, const void* parameters, unsigned parametersByteSize, unsigned operationSet) {
    //エフェクトを適用してあるか
    if (!mIsApplied) {
        Debug::logWarning("Effect not applied.");
        return;
    }
    //IDが有効か
    if (effectID == -1) {
        Debug::logWarning("Invalid Effect ID.");
        return;
    }
    //配列の範囲内か
    assert(effectID >= 0 && effectID < mDescs.size());

    auto res = mVoice.getXAudio2Voice()->SetEffectParameters(effectID, parameters, parametersByteSize, operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed set effect parameters.");
    }
#endif // _DEBUG
}
