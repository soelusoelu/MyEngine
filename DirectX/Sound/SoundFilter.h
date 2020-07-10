﻿#pragma once

#include <xaudio2.h>

class SourceVoice;

//サウンドフィルター専門クラス
class SoundFilter {
public:
    SoundFilter(SourceVoice& sourceVoice, bool useFilters);
    ~SoundFilter();

    /// <summary>
    /// ローパスフィルター
    /// 指定した値より高い周波数をカットする
    /// </summary>
    /// <param name="frequency">カットオフ周波数</param>
    /// <param name="oneOverQ">音のカットの仕方</param>
    /// <param name="operationSet">0でいい</param>
    void lowPassFilter(float frequency, float oneOverQ = 1.4142135f, unsigned operationSet = 0) const;

    /// <summary>
    /// ハイパスフィルター
    /// 指定した値より低い周波数をカットする
    /// </summary>
    /// <param name="frequency">カットオフ周波数</param>
    /// <param name="oneOverQ">音のカットの仕方</param>
    /// <param name="operationSet">0でいい</param>
    void highPassFilter(float frequency, float oneOverQ = 1.4142135f, unsigned operationSet = 0) const;

    /// <summary>
    /// バンドパスフィルタ
    /// 指定した値を中心に周りの周波数をカットする
    /// </summary>
    /// <param name="frequency">中心周波数</param>
    /// <param name="oneOverQ">音のカットの仕方</param>
    /// <param name="operationSet">0でいい</param>
    void bandPassFilter(float frequency, float oneOverQ = 1.f, unsigned operationSet = 0) const;

    /// <summary>
    /// ノッチフィルタ
    /// 指定した値を中心に周波数をカットする
    /// </summary>
    /// <param name="frequency">中心周波数</param>
    /// <param name="oneOverQ">音のカットの仕方</param>
    /// <param name="operationSet">0でいい</param>
    void notchFilter(float frequency, float oneOverQ = 1.f, unsigned operationSet = 0) const;

    /// <summary>
    /// フィルターの機能をオフにする
    /// </summary>
    void resetFilter();

private:
    SoundFilter(const SoundFilter&) = delete;
    SoundFilter& operator=(const SoundFilter&) = delete;

    //フィルタを設定する
    HRESULT setFilterParameters(XAUDIO2_FILTER_TYPE type, float frequency, float oneOverQ, unsigned operationSet) const;
    //カットオフ周波数からラジアン周波数に変換する
    float frequencyToRadianFrequency(float frequency) const;
    //Qの逆数を安全範囲内にクランプする
    float clampOneOverQ(float oneOverQ) const;
    //エフェクト使用許可が出ていない場合警告
    void printWarnigUseFilters() const;

private:
    SourceVoice& mSourceVoice;
    bool mUseFilters;
};