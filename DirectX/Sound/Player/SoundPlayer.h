#pragma once

#include "SoundBuffer.h"
#include "../Loader/ISoundLoader.h"
#include "../Loader/WaveFormat.h"
#include <xaudio2.h>
#include <memory>

class SourceVoice;
class SoundStreaming;
class Frequency;

//音の再生を扱うクラス
class SoundPlayer {
public:
    SoundPlayer(SourceVoice& sourceVoice, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format, float maxFrequencyRatio);
    ~SoundPlayer();

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// フェードインしながら、ストリーミング再生をする
    /// </summary>
    /// <param name="targetVolume">目標の音量</param>
    /// <param name="targetTime">何秒かけてフェードするか</param>
    void playStreamingFadeIn(float targetVolume, float targetTime);

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
    /// <param name="operationSet">いつ実行するか</param>
    void stop(unsigned operationSet = XAUDIO2_COMMIT_NOW) const;

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
    /// 周波数専門クラスにアクセスする
    /// </summary>
    /// <returns></returns>
    Frequency& frequency() const;

private:
    SoundPlayer(const SoundPlayer&) = delete;
    SoundPlayer& operator=(const SoundPlayer&) = delete;

private:
    SourceVoice& mSourceVoice;
    std::unique_ptr<SoundStreaming> mStreaming;
    std::unique_ptr<Frequency> mFrequency;
};
