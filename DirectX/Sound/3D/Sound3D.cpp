#include "Sound3D.h"
#include "DspSetter.h"
#include "Emitter/Sound3DEmitter.h"
#include "Listener/Sound3DListener.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../Flag/SoundFlag.h"
#include "../../DebugLayer/Debug.h"

Sound3D::Sound3D(SourceVoice& sourceVoice, const MasteringVoice& masteringVoice, const WaveFormat& format) :
    m3DInstance(),
    mListener(std::make_unique<Sound3DListener>()),
    mEmitter(std::make_unique<Sound3DEmitter>(format)),
    mDspSetter(std::make_unique<DspSetter>(sourceVoice, masteringVoice, format)),
    mInitialized(false),
    mFlags(0)
{
    auto res = X3DAudioInitialize(masteringVoice.getChannelMask(), X3DAUDIO_SPEED_OF_SOUND, m3DInstance);
    if (FAILED(res)) {
        Debug::logError("Failed 3d sound initialize.");
        return;
    }

    mInitialized = true;
}

Sound3D::~Sound3D() = default;

void Sound3D::update() {
    if (!mInitialized) {
        return;
    }

    mListener->update();
    mEmitter->update();

    calculate();
}

Sound3DListener& Sound3D::getListener() const {
    return *mListener;
}

Sound3DEmitter& Sound3D::getEmitter() const {
    return *mEmitter;
}

void Sound3D::calculate() {
    static_assert(sizeof(Vector3) == sizeof(X3DAUDIO_VECTOR), "Vector3 size does not match.");
    static_assert(sizeof(Sound3DListenerParam) == sizeof(X3DAUDIO_LISTENER), "Listener sizes do not match.");
    static_assert(sizeof(Sound3DEmitterParam) == sizeof(X3DAUDIO_EMITTER), "Emitter sizes do not match.");

    X3DAUDIO_LISTENER listener;
    memcpy(&listener, &mListener, sizeof(mListener->getListener()));
    X3DAUDIO_EMITTER emitter;
    memcpy(&emitter, &mEmitter, sizeof(mEmitter->getEmitter()));
    auto& dsp = mDspSetter->getDspSetting();
    //計算に使用するフラグを設定
    mFlags = Sound3DFlags::CALCULATE_MATRIX | Sound3DFlags::CALCULATE_DOPPLER | Sound3DFlags::CALCULATE_LPF_DIRECT | Sound3DFlags::CALCULATE_LPF_REVERB | Sound3DFlags::CALCULATE_REVERB;
    //本計算
    X3DAudioCalculate(m3DInstance, &listener, &emitter, mFlags, &dsp);

    //計算結果をソースボイスに適用する
    mDspSetter->applyToSourceVoice();
}
