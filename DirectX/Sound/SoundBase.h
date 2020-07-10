#pragma once

#include "SourceVoiceInitParam.h"
#include "SubmixVoiceInitParam.h"
#include <XAudio2.h>
#include <memory>
#include <string>

class SourceVoice;
class SoundLoader;
class SubmixVoice;

//サウンドAPIの基盤となるクラス
class SoundBase {
public:
    SoundBase();
    ~SoundBase();

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
    SoundBase(const SoundBase&) = delete;
    SoundBase& operator=(const SoundBase&) = delete;

    //COMシステムの初期化
    bool comInitialize();
    //XAudio2の生成
    bool createXAudio2();
    //マスターボイスの生成
    bool createMasteringVoice();

private:
    IXAudio2* mXAudio2;
    IXAudio2MasteringVoice* mMasteringVoice;
    bool mSucceedInitialize;

    static inline bool mInstantiated = false;
};
