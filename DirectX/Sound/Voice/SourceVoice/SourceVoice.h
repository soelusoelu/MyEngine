#pragma once

#include "SourceVoiceInitParam.h"
#include "../IVoice.h"
#include "../../Data/SoundBuffer.h"
#include "../../Loader/WaveformData.h"
#include <xaudio2.h>
#include <memory>

class MasteringVoice;
class VoiceDetails;
class SoundPlayer;
class SoundVolume;
class SoundFilter;
class SubmixVoice;

//IXAudio2SourceVoiceラッパークラス
class SourceVoice : public IVoice {
public:
    SourceVoice(IXAudio2SourceVoice* XAudio2SourceVoice, MasteringVoice& masteringVoice, const WaveformData& data, const SourceVoiceInitParam& param);
    ~SourceVoice();

    virtual IXAudio2Voice* getXAudio2Voice() const override;

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
    /// 出力先ボイスを設定する
    /// </summary>
    /// <param name="voice">設定したいボイス</param>
    /// <param name="useFilter">フィルターを使用するか</param>
    void setOutputVoice(const IVoice& voice, bool useFilter = false);

    /// <summary>
    /// バッファを返す
    /// </summary>
    /// <returns></returns>
    SoundBuffer& getSoundBuffer() const;

    /// <summary>
    /// サウンドデータを返す
    /// </summary>
    /// <returns></returns>
    VoiceDetails& getSoundData() const;

    /// <summary>
    /// サウンド再生クラスを返す
    /// </summary>
    /// <returns></returns>
    SoundPlayer& getSoundPlayer() const;

    /// <summary>
    /// サウンド音量設定クラスを返す
    /// </summary>
    /// <returns></returns>
    SoundVolume& getSoundVolume() const;

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
    std::unique_ptr<VoiceDetails> mData;
    std::unique_ptr<SoundPlayer> mSoundPlayer;
    std::unique_ptr<SoundVolume> mSoundVolume;
    std::unique_ptr<SoundFilter> mSoundFilter;
};
