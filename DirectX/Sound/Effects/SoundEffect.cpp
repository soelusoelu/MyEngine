#include "SoundEffect.h"
#include "Echo/Echo.h"
#include "Equalizer/Equalizer.h"
#include "Reverb/Reverb.h"
#include "Reverb/SimpleReverb.h"
#include "VolumeMeter/VolumeMeter.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../Voice/MasteringVoice/OutputVoiceDetails.h"
#include "../../DebugLayer/Debug.h"
#include <cassert>

SoundEffect::SoundEffect(IVoice& voice, MasteringVoice& masteringVoice) :
    mVoice(voice),
    mMasteringVoice(masteringVoice),
    mDescs(),
    mIsApplied(false) {
}

SoundEffect::~SoundEffect() {
    for (auto&& desc : mDescs) {
        desc.pEffect->Release();
        desc.pEffect = nullptr;
    }
}

int SoundEffect::reverb() {
    Reverb reverb;
    return createEffect(&reverb);
}

int SoundEffect::simpleReverb() {
    SimpleReverb reverb;
    return createEffect(&reverb);
}

int SoundEffect::echo() {
    Echo echo;
    return createEffect(&echo);
}

int SoundEffect::equalizer() {
    Equalizer equalizer;
    return createEffect(&equalizer);
}

int SoundEffect::volumeMeter() {
    XAUDIO2_EFFECT_DESCRIPTOR desc;
    desc.InitialState = true;
    desc.OutputChannels = mMasteringVoice.getDetails().outputChannels;
    desc.pEffect = reinterpret_cast<VolumeMeter::CXAPOParametersBase::CXAPOBase::IXAPO::IUnknown*>(new VolumeMeter());

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
    if (!canAccessEffects(effectID)) {
        return;
    }

    auto res = mVoice.getXAudio2Voice()->SetEffectParameters(effectID, parameters, parametersByteSize, operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed set effect parameters.");
    }
#endif // _DEBUG
}

void SoundEffect::getEffectParameters(int effectID, void* parameters, unsigned parametersByteSize) const {
    if (!canAccessEffects(effectID)) {
        return;
    }

    auto res = mVoice.getXAudio2Voice()->GetEffectParameters(effectID, parameters, parametersByteSize);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed set effect parameters.");
    }
#endif // _DEBUG
}

int SoundEffect::createEffect(ISoundEffect* target) {
    XAUDIO2_EFFECT_DESCRIPTOR desc;
    desc.InitialState = true;
    desc.OutputChannels = mMasteringVoice.getDetails().outputChannels;
    bool res = target->create(&desc);
    if (!res) { //エフェクトの作成に失敗していたら-1
        return -1;
    }

    mDescs.emplace_back(desc);
    return mDescs.size() - 1;
}

bool SoundEffect::canAccessEffects(int effectID) const {
    //エフェクトを適用してあるか
    if (!mIsApplied) {
        Debug::logWarning("Effect not applied.");
        return false;
    }
    //IDが有効か
    if (effectID == -1) {
        Debug::logWarning("Invalid Effect ID.");
        return false;
    }
    //配列の範囲内か
    assert(effectID >= 0 && effectID < mDescs.size());

    return true;
}
