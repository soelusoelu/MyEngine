#pragma once

#include "IEffectCreater.h"
#include "IEffectParameter.h"
#include "ISoundEffect.h"
#include "../Voice/IVoice.h"
#include <xaudio2.h>
#include <memory>
#include <vector>

class MasteringVoice;
class SoundFilter;

//サウンドエフェクト専用クラス
class SoundEffect : public IEffectCreater, public IEffectParameter {
public:
    SoundEffect(IVoice& voice, MasteringVoice& masteringVoice, bool useFilters);
    ~SoundEffect();

    /// <summary>
    /// エフェクトにパラメータを設定する
    /// </summary>
    /// <param name="effectID">設定するエフェクトのID</param>
    /// <param name="parameters">設定するデータの先頭アドレス</param>
    /// <param name="parametersByteSize">データ(parameters)のサイズ</param>
    /// <param name="operationSet">0でいい</param>
    virtual void setEffectParameters(int effectID, const void* parameters, unsigned parametersByteSize, unsigned operationSet = XAUDIO2_COMMIT_NOW) override;

    /// <summary>
    /// エフェクトのパラメータを取得する
    /// </summary>
    /// <param name="effectID">取得するエフェクトのID</param>
    /// <param name="parameters">パラメータ格納先の先頭アドレス</param>
    /// <param name="parametersByteSize">取得するパラメータのサイズ</param>
    virtual void getEffectParameters(int effectID, void* parameters, unsigned parametersByteSize) const override;

    /// <summary>
    /// フィルター管理クラスを返す
    /// </summary>
    /// <returns></returns>
    SoundFilter& getFilter() const;

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

private:
    SoundEffect(const SoundEffect&) = delete;
    SoundEffect& operator=(const SoundEffect&) = delete;

    //エフェクトを生成する
    int createEffect(ISoundEffect* target, bool isApply = true);
    virtual int createEffect(IUnknown* target, bool isApply = true) override;
    //作成した全エフェクトを適用する
    void apply();
    //エフェクトにアクセスできる状態か
    bool canAccessEffects(int effectID, const void* parameters) const;

private:
    IVoice& mVoice;
    std::unique_ptr<SoundFilter> mSoundFilter;
    std::vector<XAUDIO2_EFFECT_DESCRIPTOR> mDescs;
    bool mIsApplied;
    const unsigned OUTPUT_CHANNELS;
};
