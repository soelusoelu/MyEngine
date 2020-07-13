#pragma once

#include "../Sound/Loader/ISoundLoader.h"
#include "../Sound/Loader/WaveformData.h"
#include "../Sound/Voice/SourceVoice/SourceVoiceInitParam.h"
#include "../Sound/Voice/SubmixVoice/SubmixVoiceInitParam.h"
#include <memory>
#include <string>
#include <unordered_map>

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

    //まだ読み込んだことがなければ、音を読み込む
    void loadSound(std::shared_ptr<WaveformData>* data, const std::string& filePath, const SourceVoiceInitParam& param);
    //拡張子を識別し、パーサーを生成する
    std::shared_ptr<ISoundLoader> createLoaderFromFilePath(const std::string& filePath);

private:
    Directory& mDirectory;
    std::unique_ptr<SoundBase> mSoundBase;
    std::unordered_map<std::string, std::shared_ptr<WaveformData>> mSounds;

    static inline bool mInstantiated = false;
};
