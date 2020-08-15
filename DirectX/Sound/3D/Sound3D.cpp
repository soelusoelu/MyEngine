#include "Sound3D.h"
#include "DspSetter.h"
#include "Emitter/Sound3DEmitter.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../Flag/SoundFlag.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"

Sound3D::Sound3D(SourceVoice& sourceVoice, const MasteringVoice& masteringVoice, const WaveFormat& format) :
    m3DInstance(),
    mEmitter(std::make_unique<Sound3DEmitter>(format)),
    mDspSetter(std::make_unique<DspSetter>(sourceVoice, masteringVoice, format)),
    mInitialized(false)
{
    auto res = X3DAudioInitialize(masteringVoice.getChannelMask(), X3DAUDIO_SPEED_OF_SOUND, m3DInstance);
    if (FAILED(res)) {
        Debug::logError("Failed 3d sound initialize.");
        return;
    }

    mInitialized = true;
}

Sound3D::~Sound3D() = default;

void Sound3D::calculate(const Sound3DListenerParam& listenerParam) {
    if (!mInitialized) {
        return;
    }

    static_assert(sizeof(Vector3) == sizeof(X3DAUDIO_VECTOR), "Vector3 size does not match.");
    static_assert(sizeof(Sound3DListenerParam) == sizeof(X3DAUDIO_LISTENER), "Listener sizes do not match.");
    static_assert(sizeof(Sound3DEmitterParam) == sizeof(X3DAUDIO_EMITTER), "Emitter sizes do not match.");

    X3DAUDIO_LISTENER listener;
    memcpy(&listener, &listenerParam, sizeof(Sound3DListenerParam));
    X3DAUDIO_EMITTER emitter;
    memcpy(&emitter, &mEmitter->getEmitter(), sizeof(Sound3DEmitterParam));
    //必要に応じたフラグを設定する
    auto flags = setFlags();
    //本計算
    X3DAudioCalculate(m3DInstance, &listener, &emitter, flags.get(), &mDspSetter->getDspSetting());

    //計算結果をソースボイスに適用する
    mDspSetter->applyToSourceVoice(flags);
}

Sound3DEmitter& Sound3D::getEmitter() const {
    return *mEmitter;
}

Flag Sound3D::setFlags() {
    //フラグを初期化する
    Flag flags;

    //行列計算は絶対
    flags.set(X3DAUDIO_CALCULATE_MATRIX);
    //ドップラー効果
    if (mEmitter->isCalculateDoppler()) {
        flags.set(X3DAUDIO_CALCULATE_DOPPLER);
    }
    //ローパスフィルタ直接パス
    if (mEmitter->isCalculateLPFDirect()) {
        flags.set(X3DAUDIO_CALCULATE_LPF_DIRECT);
    }
    //ローパスフィルタリバーブパス
    if (mEmitter->isCalculateLPFReverb()) {
        flags.set(X3DAUDIO_CALCULATE_LPF_REVERB);
    }
    //リバーブ
    if (mEmitter->isCalculateReverb()) {
        flags.set(X3DAUDIO_CALCULATE_REVERB);
    }

    return flags;
}
