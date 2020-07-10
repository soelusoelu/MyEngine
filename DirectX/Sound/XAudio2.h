﻿#pragma once

#include "SourceVoiceInitParam.h"
#include "SubmixVoiceInitParam.h"
#include <xaudio2.h>
#include <memory>

class MasteringVoice;
class SourceVoice;
class SoundLoader;
class SubmixVoice;

//IXAudio2ラッパークラス
class XAudio2 {
public:
    XAudio2(bool* successFlag);
    ~XAudio2();

    /// <summary>
    /// マスターボイスを作成する
    /// </summary>
    /// <returns>作成したマスターボイス</returns>
    MasteringVoice* createMasteringVoice() const;

    /// <summary>
    /// ソースボイスを作成する
    /// </summary>
    /// <param name="data">事前に取得してあるサウンドデータ</param>
    /// <param name="param">初期化用パラメータ</param>
    /// <returns>作成したソースボイス</returns>
    std::unique_ptr<SourceVoice> createSourceVoice(const SoundLoader& data, const SourceVoiceInitParam& param) const;

    /// <summary>
    /// サブミックスボイスを作成する
    /// </summary>
    /// <param name="param">初期化用パラメータ</param>
    /// <returns>作成したサブミックスボイス</returns>
    std::unique_ptr<SubmixVoice> createSubmixVoice(const SubmixVoiceInitParam& param) const;

private:
    XAudio2(const XAudio2&) = delete;
    XAudio2& operator=(const XAudio2&) = delete;

    //XAudio2の生成
    bool createXAudio2();

private:
    IXAudio2* mXAudio2;

    static inline bool mInstantiated = false;
};