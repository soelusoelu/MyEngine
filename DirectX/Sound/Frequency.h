#pragma once

#include <xaudio2.h>

class SourceVoice;

//周波数を扱うクラス
class Frequency {
public:
    Frequency(SourceVoice& sourceVoice);
    ~Frequency();

    /// <summary>
    /// ボイスの再生ピッチを変える
    /// </summary>
    /// <param name="pitch">再生速度 ソースボイス作成時のピッチ上限を超える値はクリッピングされる</param>
    /// <param name="operationSet">いつ実行するか</param>
    void setFrequencyRatio(float pitch, unsigned operationSet = XAUDIO2_COMMIT_NOW);

private:
    Frequency(const Frequency&) = delete;
    Frequency& operator=(const Frequency&) = delete;

private:
    SourceVoice& mSourceVoice;
};
