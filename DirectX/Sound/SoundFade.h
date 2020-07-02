#pragma once

//サウンドフェード専門クラス
class SoundFade {
public:
    SoundFade();
    ~SoundFade();

    /// <summary>
    /// フェード指定
    /// </summary>
    /// <param name="targetVolume">目標の音量</param>
    /// <param name="targetTime">何秒かけてフェードするか</param>
    void fade(float targetVolume, float targetTime, float currentVolume);

    /// <summary>
    /// フェードを進める
    /// </summary>
    void updateFade();

private:
    float mTargetVolume;
    float mTargetTime;
    float mBeforeVolume;
    float mTimeRate;
    bool mIsFading;
};
