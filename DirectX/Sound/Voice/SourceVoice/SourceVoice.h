#pragma once

#include "SourceVoiceInitParam.h"
#include "../IVoice.h"
#include "../../Data/SoundBuffer.h"
#include "../../Loader/WaveformData.h"
#include <xaudio2.h>
#include <memory>
#include <vector>

class MasteringVoice;
class SoundPlayer;
class SoundFilter;

//IXAudio2SourceVoiceラッパークラス
class SourceVoice : public IVoice {
public:
    SourceVoice(IXAudio2SourceVoice* XAudio2SourceVoice, MasteringVoice& masteringVoice, const WaveformData& data, const SourceVoiceInitParam& param);
    ~SourceVoice();

    /// <summary>
    /// XAudio2ボイスインターフェースを返す
    /// </summary>
    /// <returns></returns>
    virtual IXAudio2Voice* getXAudio2Voice() const override;

    /// <summary>
    /// ボイスの詳細を返す
    /// </summary>
    /// <returns></returns>
    virtual const VoiceDetails& getVoiceDetails() const override;

    /// <summary>
    /// サウンド音量設定クラスを返す
    /// </summary>
    /// <returns></returns>
    virtual SoundVolume& getSoundVolume() const override;

    /// <summary>
    /// ボイスの出力先管理クラスを返す
    /// </summary>
    /// <returns></returns>
    virtual OutputVoices& getOutputVoices() const override;

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// IXAudio2SourceVoiceを返す
    /// </summary>
    /// <returns></returns>
    IXAudio2SourceVoice* getXAudio2SourceVoice() const;

    /// <summary>
    /// ソースボイスに波形データを追加する
    /// 初めての場合は最初に、既に他のデータが充填されている場合はその次の再生データとして後ろに追加される
    /// 充填できる最大はXAUDIO2_MAX_QUEUED_BUFFERSで64個
    /// </summary>
    /// <param name="buffer">追加する波形データ</param>
    void submitSourceBuffer(const SoundBuffer& buffer) const;

    /// <summary>
    /// ソースボイスに自身が管理している波形データを追加する
    /// 初めての場合は最初に、既に他のデータが充填されている場合はその次の再生データとして後ろに追加される
    /// 充填できる最大はXAUDIO2_MAX_QUEUED_BUFFERSで64個
    /// </summary>
    void submitSourceBuffer() const;

    /// <summary>
    /// バッファを返す
    /// </summary>
    /// <returns></returns>
    SoundBuffer& getSoundBuffer() const;

    /// <summary>
    /// サウンドデータを返す
    /// </summary>
    /// <returns></returns>
    SoundData& getSoundData() const;

    /// <summary>
    /// サウンド再生クラスを返す
    /// </summary>
    /// <returns></returns>
    SoundPlayer& getSoundPlayer() const;

    /// <summary>
    /// サウンドエフェクト設定クラスを返す
    /// </summary>
    /// <returns></returns>
    SoundFilter& getSoundFilter() const;

private:
    SourceVoice(const SourceVoice&) = delete;
    SourceVoice& operator=(const SourceVoice&) = delete;

    //SoundBufferからXAUDIO2_BUFFERへの変換
    XAUDIO2_BUFFER toBuffer(const SoundBuffer& buffer) const;

private:
    IXAudio2SourceVoice* mXAudio2SourceVoice;
    MasteringVoice& mMasteringVoice;
    std::unique_ptr<SoundBuffer> mSoundBuffer;
    VoiceDetails mDetails;
    std::unique_ptr<SoundData> mSoundData;
    std::unique_ptr<SoundPlayer> mSoundPlayer;
    std::unique_ptr<SoundVolume> mSoundVolume;
    std::unique_ptr<OutputVoices> mOutputVoices;
    std::unique_ptr<SoundFilter> mSoundFilter;
};
