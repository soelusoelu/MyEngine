﻿#pragma once

#define XAUDIO2_HELPER_FUNCTIONS
#include "../Voice/IVoice.h"
#include <xaudio2.h>
#include <memory>

class MasteringVoice;
class OutputVoices;
class SoundFade;
class SoundPan;

//ボイスの音量を扱うクラス
class SoundVolume {
public:
    SoundVolume(IVoice& voice, MasteringVoice& masteringVoice, OutputVoices& outputVoices);
    ~SoundVolume();

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// ボイスの音量を変更する
    /// </summary>
    /// <param name="volume">設定したい音量</param>
    /// <param name="operationSet">いつ実行するか</param>
    void setVolume(float volume, unsigned operationSet = XAUDIO2_COMMIT_NOW);

    /// <summary>
    /// デシベルで音量を変更する
    /// </summary>
    /// <param name="decibels">設定したいデシベル値</param>
    /// <param name="operationSet">いつ実行するか</param>
    void setVolumeByDecibels(float decibels, unsigned operationSet = XAUDIO2_COMMIT_NOW);

    /// <summary>
    /// ボリューム値からデシベル値に変換する
    /// </summary>
    /// <param name="volume">変換させたいボリューム値</param>
    /// <returns>変換されたデシベル値</returns>
    float amplitudeRatioToDecibels(float volume);

    /// <summary>
    /// デシベル値からボリューム値に変換する
    /// </summary>
    /// <param name="decibels">変換させたいデシベル値</param>
    /// <returns>変換されたボリューム値</returns>
    float decibelsToAmplitudeRatio(float decibels);

    /// <summary>
    /// 現在設定されている音量を返す
    /// </summary>
    /// <returns>現在の音量</returns>
    float getVolume() const;

    /// <summary>
    /// フェード専門クラスを返す
    /// </summary>
    /// <returns></returns>
    SoundFade& fade() const;

    /// <summary>
    /// パンニング専門クラスを返す
    /// </summary>
    /// <returns></returns>
    SoundPan& getSoundPan() const;

private:
    SoundVolume(const SoundVolume&) = delete;
    SoundVolume& operator=(const SoundVolume&) = delete;

private:
    IVoice& mVoice;
    std::unique_ptr<SoundFade> mFader;
    std::unique_ptr<SoundPan> mSoundPan;
    float mCurrentVolume;
};