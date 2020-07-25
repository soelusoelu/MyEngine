#pragma once

#include "ISoundEffect.h"
#include "../Voice/IVoice.h"
#include <xaudio2.h>
#include <vector>

class MasteringVoice;
class Echo;

//サウンドエフェクト専用クラス
class SoundEffect {
public:
    SoundEffect(IVoice& voice, MasteringVoice& masteringVoice);
    ~SoundEffect();

    /// <summary>
    /// リバーブ効果(残響)を掛ける
    /// </summary>
    /// <returns>作成したエフェクトのID 失敗したら-1</returns>
    int reverb();

    /// <summary>
    /// 簡易版リバーブを掛ける
    /// </summary>
    /// <returns>作成したエフェクトのID 失敗したら-1</returns>
    int simpleReverb();

    /// <summary>
    /// エコーを掛ける
    /// </summary>
    /// <returns>作成したエフェクトのID 失敗したら-1</returns>
    int echo();

    /// <summary>
    /// イコライザー
    /// </summary>
    /// <returns>作成したエフェクトのID 失敗したら-1</returns>
    int equalizer();

    /// <summary>
    /// ボリュームメーター
    /// </summary>
    /// <returns>作成したエフェクトのID 失敗したら-1</returns>
    int volumeMeter();

    /// <summary>
    /// 作成した全エフェクトを適用する
    /// </summary>
    void apply();

    /// <summary>
    /// エフェクトにパラメータを設定する
    /// </summary>
    /// <param name="effectID">設定するエフェクトのID</param>
    /// <param name="parameters">設定するデータの先頭アドレス</param>
    /// <param name="parametersByteSize">データ(parameters)のサイズ</param>
    /// <param name="operationSet">0でいい</param>
    void setEffectParameters(int effectID, const void* parameters, unsigned parametersByteSize, unsigned operationSet = XAUDIO2_COMMIT_NOW);

    /// <summary>
    /// エフェクトのパラメータを取得する
    /// </summary>
    /// <param name="effectID">取得するエフェクトのID</param>
    /// <param name="parameters">パラメータ格納先の先頭アドレス</param>
    /// <param name="parametersByteSize">取得するパラメータのサイズ</param>
    void getEffectParameters(int effectID, void* parameters, unsigned parametersByteSize) const;

private:
    SoundEffect(const SoundEffect&) = delete;
    SoundEffect& operator=(const SoundEffect&) = delete;

    //エフェクトを生成する
    int createEffect(ISoundEffect* target);
    //エフェクトにアクセスできる状態か
    bool canAccessEffects(int effectID, const void* parameters) const;

private:
    IVoice& mVoice;
    MasteringVoice& mMasteringVoice;
    std::vector<XAUDIO2_EFFECT_DESCRIPTOR> mDescs;
    bool mIsApplied;
};
