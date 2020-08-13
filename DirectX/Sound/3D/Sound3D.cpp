#include "Sound3D.h"
#include "../Voice/VoiceDetails.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../../Component/Camera/Camera.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"
#include "../../Transform/Transform3D.h"

Sound3D::Sound3D(const MasteringVoice& masteringVoice, const WaveFormat& format) :
    m3DInstance(),
    mListener(),
    mEmitter(),
    mDspSettings(),
    mInitialized(false),
    mEmitterPreviousPos(Vector3::zero)
{
    auto res = X3DAudioInitialize(masteringVoice.getChannelMask(), X3DAUDIO_SPEED_OF_SOUND, m3DInstance);
    if (FAILED(res)) {
        Debug::logError("Failed 3d sound initialize.");
        return;
    }

    //入力・出力チャンネル数の取得
    const auto inCh = format.channels;
    const auto outCh = masteringVoice.getVoiceDetails().channels;

    //リスナー
    mListener.cone = &LISTENER_DIRECTIONAL_CONE;
    //エミッター
    mEmitter.cone = &EMITTER_CONE;
    mEmitter.channelCount = inCh;
    mEmitter.channelRadius = 1.f;
    //mEmitter.channelAzimuths = 
    mEmitter.innerRadius = 2.f;
    mEmitter.innerRadiusAngle = Math::PI / 4.f;
    mEmitter.volumeCurve = &DEFAULT_LINEAR_CURVE;
    mEmitter.lfeCurve = &EMITTER_LFE_CURVE;
    mEmitter.lpfDirectCurve = nullptr; //デフォルトカーブを使用
    mEmitter.lpfReverbCurve = nullptr; //デフォルトカーブを使用
    mEmitter.reverbCurve = &EMITTER_REVERB_CURVE;
    mEmitter.curveDistanceScaler = 14.f;
    mEmitter.dopplerScaler = 1.f;
    //dsp
    mDspSettings.SrcChannelCount = inCh;
    mDspSettings.DstChannelCount = outCh;
    mMatrixCoefficients.resize(inCh * outCh);
    mDspSettings.pMatrixCoefficients = mMatrixCoefficients.data();

    mInitialized = true;
}

Sound3D::~Sound3D() = default;

void Sound3D::update() {
    mEmitterPreviousPos = mEmitter.position;
    calculate();
}

void Sound3D::setListenerByCamera(const Camera& camera) {
    mListener.orientFront = camera.transform().forward();
    mListener.orientTop = camera.transform().up();
    mListener.position = camera.getPosition();
}

void Sound3D::setEmitterPosition(const Vector3& pos) {
    mEmitter.position = pos;
}

void Sound3D::calculate() {
    if (!mInitialized) {
        return;
    }

    X3DAUDIO_LISTENER listener;
    memcpy(&listener, &mListener, sizeof(mListener));
    X3DAUDIO_EMITTER emitter;
    memcpy(&emitter, &mEmitter, sizeof(mEmitter));
    constexpr unsigned flags = Sound3DFlags::CALCULATE_MATRIX | Sound3DFlags::CALCULATE_DOPPLER | Sound3DFlags::CALCULATE_LPF_DIRECT | Sound3DFlags::CALCULATE_LPF_REVERB | Sound3DFlags::CALCULATE_REVERB;
    X3DAudioCalculate(m3DInstance, &listener, &emitter, flags, &mDspSettings);
}
