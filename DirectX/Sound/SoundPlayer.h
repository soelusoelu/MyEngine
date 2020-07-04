#pragma once

#include "SoundBuffer.h"
#include <xaudio2.h>

class SourceVoice;

//音の再生を扱うクラス
class SoundPlayer {
public:
    SoundPlayer(SourceVoice& sourceVoice);
    ~SoundPlayer();

    /// <summary>
    /// ソースボイスに設定されているサウンドを再生する
    /// </summary>
    /// <param name="flags">0でいい</param>
    /// <param name="operationSet">いつ実行するか</param>
    void play(unsigned flags = 0, unsigned operationSet = XAUDIO2_COMMIT_NOW) const;

    /// <summary>
    /// フェードインしながら、ソースボイスに設定されているサウンドを再生する
    /// </summary>
    /// <param name="targetVolume">目標の音量</param>
    /// <param name="targetTime">何秒かけてフェードするか</param>
    /// <param name="flags">0でいい</param>
    /// <param name="operationSet">いつ実行するか</param>
    void playFadeIn(float targetVolume, float targetTime, unsigned flags = 0, unsigned operationSet = XAUDIO2_COMMIT_NOW) const;

    /// <summary>
    /// ソースボイスに設定されているサウンドをループ再生する
    /// </summary>
    /// <param name="flags">0でいい</param>
    /// <param name="operationSet">いつ実行するか</param>
    void playInfinity(unsigned flags = 0, unsigned operationSet = XAUDIO2_COMMIT_NOW) const;

    /// <summary>
    /// 再生を即一時停止する
    /// </summary>
    /// <param name="flags">XAUDIO2_PLAY_TAILSはエフェクトの効果は再生し続けるというもの</param>
    /// <param name="operationSet">いつ実行するか</param>
    void pause(unsigned flags = XAUDIO2_PLAY_TAILS, unsigned operationSet = XAUDIO2_COMMIT_NOW) const;

    /// <summary>
    /// フェードアウトしながら再生を一時停止する
    /// </summary>
    /// <param name="targetTime">何秒かけてフェードするか</param>
    void pauseFadeOut(float targetTime) const;

    /// <summary>
    /// 再生を即停止する
    /// </summary>
    /// <param name="flags">0でいい</param>
    /// <param name="operationSet">いつ実行するか</param>
    void stop(unsigned flags = 0, unsigned operationSet = XAUDIO2_COMMIT_NOW) const;

    /// <summary>
    /// フェードアウトしながら再生を停止する
    /// </summary>
    /// <param name="targetTime">何秒かけてフェードするか</param>
    void stopFadeOut(float targetTime) const;

    /// <summary>
    /// 再生が停止しているか
    /// </summary>
    /// <returns></returns>
    bool isStop() const;

    /// <summary>
    /// ループを終了する(再生は続く)
    /// </summary>
    /// <param name="operationSet">いつ実行するか</param>
    void exitLoop(unsigned operationSet = XAUDIO2_COMMIT_NOW) const;

private:
    SoundPlayer(const SoundPlayer&) = delete;
    SoundPlayer& operator=(const SoundPlayer&) = delete;

    /// <summary>
    /// 引数のバッファからサウンドを再生する
    /// </summary>
    /// <param name="buffer">波形データ</param>
    /// <param name="flags">0でいい</param>
    /// <param name="operationSet">いつ実行するか</param>
    void playFromSoundBuffer(const SoundBuffer& buffer, unsigned flags = 0, unsigned operationSet = XAUDIO2_COMMIT_NOW) const;

private:
    SourceVoice& mSourceVoice;
};
