#pragma once

#include <string>

//波形ファイル出力クラス
class WaveformOutput {
public:
    /// <summary>
    /// 波形をファイル出力する
    /// </summary>
    /// <param name="filePath">ファイルパス</param>
    /// <param name="waveform">波形</param>
    /// <param name="size">波形の長さ</param>
    static void outputWaveform(const std::string& filePath, const float* waveform, unsigned size);

private:
    WaveformOutput() = delete;
    ~WaveformOutput() = delete;
};
