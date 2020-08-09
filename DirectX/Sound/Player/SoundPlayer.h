#pragma once

#include "SoundBuffer.h"
#include "../Loader/ISoundLoader.h"
#include "../Loader/WaveFormat.h"
#include <xaudio2.h>
#include <memory>

class SourceVoice;
class SoundStreaming;
class SoundLoop;
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
    /// 再生開始地点を設定する
    /// </summary>
    /// <param name="point">再生開始地点(単位: 秒数)</param>
    void setPlayPoint(float point);

    /// <summary>
    /// 再生を一時停止する
    /// </summary>
    void pause() const;

    /// <summary>
    /// フェードアウトしながら再生を一時停止する
    /// </summary>
    /// <param name="targetTime">何秒かけてフェードするか</param>
    void pauseFadeOut(float targetTime) const;

    /// <summary>
    /// 再生を停止する
    /// </summary>
    void stop() const;

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
    /// ループ専門クラスにアクセスする
    /// </summary>
    /// <returns></returns>
    SoundLoop& loop() const;

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
    std::unique_ptr<SoundLoop> mLoop;
    std::unique_ptr<Frequency> mFrequency;
};
