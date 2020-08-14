#include "Sound3D.h"
#include "DspSetter.h"
#include "Emitter/Sound3DEmitter.h"
#include "Listener/Sound3DListener.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../Flag/SoundFlag.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"

Sound3D::Sound3D(SourceVoice& sourceVoice, const MasteringVoice& masteringVoice, const WaveFormat& format) :
    m3DInstance(),
    mListener(std::make_unique<Sound3DListener>()),
    mEmitter(std::make_unique<Sound3DEmitter>(format)),
    mDspSetter(std::make_unique<DspSetter>(sourceVoice, masteringVoice, format)),
    mInitialized(false),
    mFlags()
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
    memcpy(&listener, &mListener->getListener(), sizeof(Sound3DListenerParam));
    X3DAUDIO_EMITTER emitter;
    memcpy(&emitter, &mEmitter->getEmitter(), sizeof(Sound3DEmitterParam));
    //必要に応じたフラグを設定する
    setFlags();
    //本計算
    X3DAudioCalculate(m3DInstance, &listener, &emitter, mFlags.get(), &mDspSetter->getDspSetting());

    //計算結果をソースボイスに適用する
    mDspSetter->applyToSourceVoice(mFlags);
}

void Sound3D::setFlags() {
    //フラグを初期化する
    mFlags.clear();

    //行列計算は絶対
    mFlags.set(X3DAUDIO_CALCULATE_MATRIX);
    //ドップラー効果
    if (mEmitter->isCalculateDoppler()) {
        mFlags.set(X3DAUDIO_CALCULATE_DOPPLER);
    }
    //ローパスフィルタ直接パス
    if (mEmitter->isCalculateLPFDirect()) {
        mFlags.set(X3DAUDIO_CALCULATE_LPF_DIRECT);
    }
    //ローパスフィルタリバーブパス
    if (mEmitter->isCalculateLPFReverb()) {
        mFlags.set(X3DAUDIO_CALCULATE_LPF_REVERB);
    }
    //リバーブ
    if (mEmitter->isCalculateReverb()) {
        mFlags.set(X3DAUDIO_CALCULATE_REVERB);
    }
}
