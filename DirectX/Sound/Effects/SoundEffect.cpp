#include "SoundEffect.h"
#include "Echo/Echo.h"
#include "Equalizer/Equalizer.h"
#include "Filter/SoundFilter.h"
#include "PlayTimer/PlayTimer.h"
#include "Reverb/Reverb.h"
#include "Reverb/SimpleReverb.h"
#include "VolumeMeter/VolumeMeter.h"
#include "../Voice/VoiceDetails.h"
#include "../../DebugLayer/Debug.h"
#include <cassert>

SoundEffect::SoundEffect(IVoice& voice, bool useFilters) :
    mVoice(voice),
    mSoundFilter(std::make_unique<SoundFilter>(*this, *this, voice, useFilters)),
    mDescs(),
    mIsApplied(false) {
}

SoundEffect::~SoundEffect() {
    for (auto&& desc : mDescs) {
        desc.pEffect->Release();
        desc.pEffect = nullptr;
    }
}

void SoundEffect::setEffectParameters(int effectID, const void* parameters, unsigned parametersByteSize, unsigned operationSet) {
    if (!canAccessEffect(effectID, parameters)) {
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
    if (!canAccessEffect(effectID, parameters)) {
        return;
    }

    auto res = mVoice.getXAudio2Voice()->GetEffectParameters(effectID, parameters, parametersByteSize);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed set effect parameters.");
    }
#endif // _DEBUG
}

void SoundEffect::enable(int effectID, unsigned operationSet) {
    if (!isValidID(effectID)) {
        return;
    }

    auto res = mVoice.getXAudio2Voice()->EnableEffect(effectID, operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed enable effect.");
    }
#endif // _DEBUG
}

void SoundEffect::disable(int effectID, unsigned operationSet) {
    if (!isValidID(effectID)) {
        return;
    }

    auto res = mVoice.getXAudio2Voice()->DisableEffect(effectID, operationSet);
#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed disable effect.");
    }
#endif // _DEBUG
}

SoundFilter& SoundEffect::getFilter() const {
    return *mSoundFilter;
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
    return createEffect(reinterpret_cast<IUnknown*>(new VolumeMeter()));
}

void SoundEffect::playTimer() {
    auto id = createEffect(reinterpret_cast<IUnknown*>(new PlayTimer()));
    assert(id == PLAY_TIMER_ID);
}

int SoundEffect::createEffect(ISoundEffect* target, bool isApply) {
    XAUDIO2_EFFECT_DESCRIPTOR desc;
    desc.InitialState = true;
    desc.OutputChannels = mVoice.getVoiceDetails().channels;
    bool res = target->create(&desc);
    if (!res) { //エフェクトの作成に失敗していたら-1
        return -1;
    }

    mDescs.emplace_back(desc);
    mIsApplied = false;

    if (isApply) {
        apply();
    }

    return mDescs.size() - 1;
}

int SoundEffect::createEffect(IUnknown* target, bool isApply) {
    if (!target) {
        return -1;
    }

    XAUDIO2_EFFECT_DESCRIPTOR desc;
    desc.InitialState = true;
    desc.OutputChannels = mVoice.getVoiceDetails().channels;
    desc.pEffect = target;

    mDescs.emplace_back(desc);
    mIsApplied = false;

    if (isApply) {
        apply();
    }

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

bool SoundEffect::isValidID(int effectID) const {
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

bool SoundEffect::isValidParameters(const void* parameters) const {
    if (!parameters) {
        Debug::logWarning("Effect parameter is null");
        return false;
    }

    return true;
}

bool SoundEffect::canAccessEffect(int effectID, const void* parameters) const {
    if (!isValidID(effectID)) {
        return false;
    }
    if (!isValidParameters(parameters)) {
        return false;
    }

    return true;
}
