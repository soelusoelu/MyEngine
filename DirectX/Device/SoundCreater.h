#pragma once

#include "../Sound/Loader/ISoundLoader.h"
#include "../Sound/Loader/WaveFormat.h"
#include "../Sound/Voice/SourceVoice/SourceVoiceInitParam.h"
#include "../Sound/Voice/SubmixVoice/SubmixVoiceInitParam.h"
#include <xaudio2.h>
#include <memory>
#include <string>

class Directory;
class SoundBase;
class SourceVoice;
class SubmixVoice;

//サウンド生成クラス
class SoundCreater {
public:
    SoundCreater(Directory& directory);
    ~SoundCreater();

    /// <summary>
    /// ソースボイスを作成する
    /// </summary>
    /// <param name="filePath">サウンドが置いてあるファイルパス</param>
    /// <param name="param">初期化用パラメータ</param>
    /// <returns>作成したソースボイス</returns>
    std::unique_ptr<SourceVoice> createSourceVoice(const std::string& filePath, const SourceVoiceInitParam& param);

    /// <summary>
    /// サブミックスボイスを作成する
    /// </summary>
    /// <param name="param">初期化用パラメータ</param>
    /// <returns>作成したサブミックスボイス</returns>
    std::unique_ptr<SubmixVoice> createSubmixVoice(const SubmixVoiceInitParam& param) const;

private:
    SoundCreater(const SoundCreater&) = delete;
    SoundCreater& operator=(const SoundCreater&) = delete;

    //拡張子を識別し、パーサーを生成する
    std::unique_ptr<ISoundLoader> createLoaderFromFilePath(const std::string& filePath);

private:
    Directory& mDirectory;
    std::unique_ptr<SoundBase> mSoundBase;

    static inline bool mInstantiated = false;
};
