#pragma once

#include "SoundBuffer.h"
#include <xaudio2.h>

//IXAudio2SourceVoiceラッパークラス
class SourceVoice {
public:
    SourceVoice();
    ~SourceVoice();

    /// <summary>
    /// ソースボイスに設定されているサウンドを鳴らす
    /// </summary>
    /// <param name="flags">0でいい</param>
    /// <param name="operationSet">いつ実行するか</param>
    void play(unsigned flags = 0, unsigned operationSet = XAUDIO2_COMMIT_NOW) const;

    /// <summary>
    /// 一時停止する
    /// </summary>
    /// <param name="flags">0でいい</param>
    /// <param name="operationSet">いつ実行するか</param>
    void stop(unsigned flags = 0, unsigned operationSet = XAUDIO2_COMMIT_NOW) const;

    /// <summary>
    /// ソースボイスに波形データを追加する
    /// 初めての場合は最初に、既に他のデータが充填されている場合はその次の再生データとして後ろに追加される
    /// 充填できる最大はXAUDIO2_MAX_QUEUED_BUFFERSで64個
    /// </summary>
    /// <param name="buffer">追加する波形データ</param>
    void submitSourceBuffer(const SoundBuffer& buffer) const;

    /// <summary>
    /// ボイスの音量を変更する
    /// </summary>
    /// <param name="volume">設定したい音量</param>
    /// <param name="operationSet">いつ実行するか</param>
    void setVolume(float volume, unsigned operationSet = XAUDIO2_COMMIT_NOW) const;

private:
    //SoundBufferからXAUDIO2_BUFFERへの変換
    XAUDIO2_BUFFER toBuffer(const SoundBuffer& buffer) const;

private:
    IXAudio2SourceVoice* mSourceVoice;
};
