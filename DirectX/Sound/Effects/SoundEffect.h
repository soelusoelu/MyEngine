#pragma once

#include "../Voice/IVoice.h"
#include <xaudio2.h>
#include <vector>

class MasteringVoice;

//サウンドエフェクト専用クラス
class SoundEffect {
public:
    SoundEffect(IVoice& voice, MasteringVoice& masteringVoice);
    ~SoundEffect();

    /// <summary>
    /// リバーブ効果(残響)を掛ける
    /// </summary>
    /// <param name="initialState">初期状態でエフェクトを有効化するか</param>
    /// <returns>作成したエフェクトのID 失敗したら-1</returns>
    int reverb(bool initialState = true);

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

private:
    SoundEffect(const SoundEffect&) = delete;
    SoundEffect& operator=(const SoundEffect&) = delete;

private:
    IVoice& mVoice;
    MasteringVoice& mMasteringVoice;
    std::vector<XAUDIO2_EFFECT_DESCRIPTOR> mDescs;
};
