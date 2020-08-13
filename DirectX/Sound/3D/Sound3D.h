#pragma once

#include "DistanceCurve.h"
#include "Sound3DCone.h"
#include "Sound3DEmitter.h"
#include "Sound3DListener.h"
#include "../Loader/WaveFormat.h"
#include "../../Math/Math.h"
#include <xaudio2.h>
#include <x3daudio.h>
#include <memory>
#include <vector>

class SourceVoice;
class MasteringVoice;
class Camera;

//3Dサウンドメインクラス
class Sound3D {
public:
    Sound3D(SourceVoice& sourceVoice, const MasteringVoice& masteringVoice, const WaveFormat& format);
    ~Sound3D();

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// カメラをもとにリスナーを設定する
    /// </summary>
    /// <param name="camera">カメラコンポーネント</param>
    void setListenerByCamera(const Camera& camera);

    /// <summary>
    /// エミッターの位置を設定する
    /// </summary>
    /// <param name="pos">エミッターの位置</param>
    void setEmitterPosition(const Vector3& pos);

private:
    Sound3D(const Sound3D&) = delete;
    Sound3D& operator=(const Sound3D&) = delete;

    //3Dサウンドを計算する
    void calculate();
    //ソースボイスに3D計算した結果を適用する
    void applyToSourceVoice();

private:
    SourceVoice& mSourceVoice;
    //3Dサウンドのハンドル
    X3DAUDIO_HANDLE m3DInstance;
    //リスナー(聞き手)
    Sound3DListener mListener;
    //エミッター(音源)
    Sound3DEmitter mEmitter;
    //結果
    X3DAUDIO_DSP_SETTINGS mDspSettings;
    //エミッターの方位角
    std::vector<float> mEmitterAzimuths;
    //行列係数
    std::vector<float> mMatrixCoefficients;
    //初期化に成功しているか
    bool mInitialized;
    //前フレームでの位置
    Vector3 mEmitterPreviousPos;

    //リスナーコーン
    static inline Sound3DCone LISTENER_DIRECTIONAL_CONE = { Math::PI * 5.f / 6.f, Math::PI * 11.f / 6.f, 1.f, 0.75f, 0.f, 0.25f, 0.708f, 1.f };
    //エミッターコーン
    static inline Sound3DCone EMITTER_CONE = { 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f };
    //通常カーブ
    static inline DistanceCurvePoint DEFAULT_LINEAR_CURVE_POINTS[2] = { 0.f, 1.f, 1.f, 0.f };
    static inline DistanceCurve DEFAULT_LINEAR_CURVE = { &DEFAULT_LINEAR_CURVE_POINTS[0], 2 };
    //LFEカーブ
    static inline DistanceCurvePoint EMITTER_LFE_CURVE_POINTS[3] = { 0.f, 1.f, 0.25f, 0.f, 1.f, 0.f };
    static inline DistanceCurve EMITTER_LFE_CURVE = { &EMITTER_LFE_CURVE_POINTS[0], 3 };
    //リバーブカーブ
    static inline DistanceCurvePoint EMITTER_REVERB_CURVE_POINTS[3] = { 0.f, 0.5f, 0.75f, 1.f, 1.f, 0.f };
    static inline DistanceCurve EMITTER_REVERB_CURVE = { &EMITTER_LFE_CURVE_POINTS[0], 3 };
};

//X3DAUDIOとの整合性チェック
static_assert(sizeof(Vector3) == sizeof(X3DAUDIO_VECTOR));
static_assert(sizeof(Sound3DListener) == sizeof(X3DAUDIO_LISTENER));
static_assert(sizeof(Sound3DEmitter) == sizeof(X3DAUDIO_EMITTER));
