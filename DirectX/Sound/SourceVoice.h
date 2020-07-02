#pragma once

#include "SoundBuffer.h"
#include <xaudio2.h>
#include <memory>

class SoundLoader;
class VoiceDetails;
class SoundPlayer;
class SoundVolume;

//IXAudio2SourceVoiceラッパークラス
class SourceVoice {
public:
    SourceVoice(IXAudio2SourceVoice* XAudio2SourceVoice, const std::shared_ptr<SoundLoader>& data);
    ~SourceVoice();

    /// <summary>
    /// IXAudio2SourceVoiceを返す
    /// </summary>
    /// <returns>IXAudio2SourceVoice</returns>
    IXAudio2SourceVoice* getXAudio2SourceVoice() const;

    /// <summary>
    /// サウンドデータを返す
    /// </summary>
    /// <returns></returns>
    VoiceDetails& getSoundData() const;

    /// <summary>
    /// ソースボイスに波形データを追加する
    /// 初めての場合は最初に、既に他のデータが充填されている場合はその次の再生データとして後ろに追加される
    /// 充填できる最大はXAUDIO2_MAX_QUEUED_BUFFERSで64個
    /// </summary>
    /// <param name="buffer">追加する波形データ</param>
    void submitSourceBuffer(const SoundBuffer& buffer) const;

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

private:
    SourceVoice(const SourceVoice&) = delete;
    SourceVoice& operator=(const SourceVoice&) = delete;

    //SoundBufferからXAUDIO2_BUFFERへの変換
    XAUDIO2_BUFFER toBuffer(const SoundBuffer& buffer) const;

private:
    IXAudio2SourceVoice* mXAudio2SourceVoice;
    std::unique_ptr<VoiceDetails> mData;
    std::unique_ptr<SoundPlayer> mSoundPlayer;
    std::unique_ptr<SoundVolume> mSoundVolume;
};
