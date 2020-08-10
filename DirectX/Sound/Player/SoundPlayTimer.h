#pragma once

class SourceVoice;

//再生時間を扱うクラス
class SoundPlayTimer {
public:
    SoundPlayTimer(SourceVoice& sourceVoice);
    ~SoundPlayTimer();

    /// <summary>
    /// 再生時間を設定する
    /// </summary>
    /// <param name="time">再生時間(単位: 秒)</param>
    void setPlayTime(float time);

    /// <summary>
    /// 再生時間を返す(単位: 秒)
    /// あまり正確ではない
    /// </summary>
    /// <returns></returns>
    float getPlayTime() const;

    /// <summary>
    /// 再生時間を進める
    /// </summary>
    void startTimer();

    /// <summary>
    /// 再生時間を止める
    /// </summary>
    void stopTimer();

private:
    SoundPlayTimer(const SoundPlayTimer&) = delete;
    SoundPlayTimer& operator=(const SoundPlayTimer&) = delete;

private:
    SourceVoice& mSourceVoice;
};
