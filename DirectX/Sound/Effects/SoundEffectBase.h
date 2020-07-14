#pragma once

#include "../Voice/IVoice.h"
#include "../../System/SystemInclude.h"
#include <xaudio2.h>

class MasteringVoice;

//サウンドエフェクト基底クラス
class SoundEffectBase {
public:
    SoundEffectBase(IVoice& voice, MasteringVoice& masteringVoice);
    virtual ~SoundEffectBase();

    /// <summary>
    /// エフェクト作成純粋仮想関数
    /// </summary>
    /// <param name="desc">XAPO設定用デスクリプタ</param>
    /// <param name="initialState">初期状態でエフェクトを有効化するか</param>
    /// <returns>エフェクトの作成に成功したか</returns>
    virtual bool create(XAUDIO2_EFFECT_DESCRIPTOR* desc, bool initialState = true) = 0;

private:
    SoundEffectBase(const SoundEffectBase&) = delete;
    SoundEffectBase& operator=(const SoundEffectBase&) = delete;

protected:
    IVoice& mVoice;
    MasteringVoice& mMasteringVoice;
    Microsoft::WRL::ComPtr<IUnknown> mXAPO;
};
