#pragma once

#include <xaudio2.h>

class SourceVoice;

//サウンドエフェクト専門クラス
class SoundEffect {
public:
    SoundEffect(SourceVoice& sourceVoice);
    ~SoundEffect();

    /// <summary>
    /// ローパスフィルター
    /// 指定した値より高い周波数をカットする
    /// </summary>
    /// <param name="frequency">周波数(この値より先をカットする)</param>
    /// <param name="oneOverQ">どのくらいの音量がカットされているか</param>
    /// <param name="operationSet">0でいい</param>
    void lowPassFilter(float frequency, float oneOverQ = 1.4142135f, unsigned operationSet = 0) const;

    /// <summary>
    /// ローパスフィルターの機能をオフにする
    /// </summary>
    void resetLowPassFilter();

private:
    SourceVoice& mSourceVoice;
};
