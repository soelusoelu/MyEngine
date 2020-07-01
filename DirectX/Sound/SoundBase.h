#pragma once

#include <XAudio2.h>
#include <memory>
#include <string>

class Sound;
class SoundLoader;

//サウンドAPIの基盤となるクラス
class SoundBase {
public:
    SoundBase();
    ~SoundBase();
    /// <summary>
    /// ソースボイスを作成する
    /// </summary>
    /// <param name="sourceVoice">インスタンス受け取り</param>
    /// <param name="sourceFormat">ボイスの形式を指定する</param>
    /// <param name="flags">ボイスに属性を設定するフラグ</param>
    /// <param name="maxFrequencyRatio">ボイスの最大許容再生速度を設定する 最大XAUDIO2_MAX_FREQ_RATIO(1024倍)まで可能</param>
    /// <param name="callback">コールバック</param>
    /// <param name="sendList">ボイスの出力先を設定する nullだとマスターボイス直結になる 後からsetOutputVoicesで設定可能</param>
    /// <param name="effectChain">エフェクト指定 nullだと使用しない 後からsetEffectChainsで設定可能</param>
    void createSourceVoice(
        IXAudio2SourceVoice** sourceVoice,
        const WAVEFORMATEX* sourceFormat,
        unsigned flags = 0,
        float maxFrequencyRatio = 2.f,
        IXAudio2VoiceCallback* callback = nullptr,
        const XAUDIO2_VOICE_SENDS* sendList = nullptr,
        const XAUDIO2_EFFECT_CHAIN* effectChain = nullptr
    ) const;

    void createSourceVoice(std::shared_ptr<Sound>* sound, const std::shared_ptr<SoundLoader>& data) const;

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
