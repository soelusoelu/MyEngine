﻿#pragma once

#include "Sound3DEmitterParam.h"
#include "../DistanceCurve.h"
#include "../Sound3DCone.h"
#include "../../Loader/WaveFormat.h"
#include "../../../Math/Math.h"
#include <vector>

//3Dサウンドで扱うエミッター(音源)
class Sound3DEmitter {
public:
    Sound3DEmitter(const WaveFormat& format);
    ~Sound3DEmitter();

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// 位置を設定する
    /// </summary>
    /// <param name="pos">エミッターの位置</param>
    void setPosition(const Vector3& pos);

    /// <summary>
    /// ドップラー効果の掛かり具合を設定する
    /// </summary>
    /// <param name="scale">ドップラーレベル</param>
    void setDopplerScale(float scale);

    /// <summary>
    /// ドップラー効果を計算するか
    /// </summary>
    /// <returns></returns>
    bool isCalculateDoppler() const;

    void setCalculateLPFDirect(bool value);
    bool isCalculateLPFDirect() const;

    void setCalculateLPFReverb(bool value);
    bool isCalculateLPFReverb() const;

    void setCalculateReverb(bool value);
    bool isCalculateReverb() const;

    /// <summary>
    /// エミッターを返す
    /// </summary>
    /// <returns></returns>
    const Sound3DEmitterParam& getEmitter() const;

private:
    Sound3DEmitter(const Sound3DEmitter&) = delete;
    Sound3DEmitter& operator=(const Sound3DEmitter&) = delete;

private:
    //エミッター
    Sound3DEmitterParam mEmitter;
    //方位角
    std::vector<float> mAzimuths;
    //前フレームでの位置
    Vector3 mPreviousPos;
    //ローパスフィルタ直接パス計算フラグ
    bool mIsCalculateLPFDirect;
    //ローパスフィルタリバーブパス計算フラグ
    bool mIsCalculateLPFReverb;
    //リバーブパス計算フラグ
    bool mIsCalculateReverb;

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