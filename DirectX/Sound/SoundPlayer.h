#pragma once

#include <xaudio2.h>

class SourceVoice;

class SoundPlayer {
public:
    SoundPlayer(SourceVoice& sourceVoice);
    ~SoundPlayer();

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
    /// 再生が終了しているか
    /// </summary>
    /// <returns></returns>
    bool isFinished() const;

private:
    SoundPlayer(const SoundPlayer&) = delete;
    SoundPlayer& operator=(const SoundPlayer&) = delete;

private:
    SourceVoice& mSourceVoice;
};
