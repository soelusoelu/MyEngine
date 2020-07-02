#pragma once

class SoundVolume;

//サウンドフェード専門クラス
class SoundFade {
public:
    SoundFade(SoundVolume& soundVolume);
    ~SoundFade();

    /// <summary>
    /// フェード指定
    /// </summary>
    /// <param name="targetVolume">目標の音量</param>
    /// <param name="targetTime">何秒かけてフェードするか</param>
    void settings(float targetVolume, float targetTime);

    /// <summary>
    /// フェードを進める
    /// </summary>
    void updateFade();

private:
    SoundFade(const SoundFade&) = delete;
    SoundFade& operator=(const SoundFade&) = delete;

private:
    SoundVolume& mSoundVolume;
    float mTargetVolume;
    float mTargetTime;
    float mBeforeVolume;
    float mTimeRate;
    bool mIsFading;
};
