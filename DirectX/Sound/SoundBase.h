#pragma once

#include <XAudio2.h>
#include <memory>
#include <string>

class SourceVoice;
class SoundLoader;

//サウンドAPIの基盤となるクラス
class SoundBase {
public:
    SoundBase();
    ~SoundBase();

    /// <summary>
    /// ソースボイスを作成する
    /// </summary>
    /// <param name="data">事前に取得してあるサウンドデータ</param>
    /// <param name="flags">ボイスに属性を設定するフラグ</param>
    /// <param name="maxFrequencyRatio">ボイスの最大許容再生速度を設定する 最大XAUDIO2_MAX_FREQ_RATIO(1024倍)まで可能</param>
    /// <param name="callback">コールバック</param>
    /// <param name="sendList">ボイスの出力先を設定する nullだとマスターボイス直結になる 後からsetOutputVoicesで設定可能</param>
    /// <param name="effectChain">エフェクト指定 nullだと使用しない 後からsetEffectChainsで設定可能</param>
    /// <returns></returns>
    std::shared_ptr<SourceVoice> createSourceVoice(
        const SoundLoader& data,
        unsigned flags,
        float maxFrequencyRatio,
        IXAudio2VoiceCallback* callback,
        const XAUDIO2_VOICE_SENDS* sendList,
        const XAUDIO2_EFFECT_CHAIN* effectChain
    ) const;

private:
    SoundBase(const SoundBase&) = delete;
    SoundBase& operator=(const SoundBase&) = delete;

    //COMシステムの初期化
    bool comInitialize();
    //XAudio2の生成
    bool createXAudio2();
    //マスターボイスの生成
    bool createMasteringVoice();

private:
    IXAudio2* mXAudio2;
    IXAudio2MasteringVoice* mMasteringVoice;
    bool mSucceedInitialize;

    static inline bool mInstantiated = false;
};
