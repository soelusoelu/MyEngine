#pragma once

#include "ISoundLoader.h"
#include "SourceVoiceInitParam.h"
#include "WaveformData.h"
#include <memory>
#include <string>
#include <unordered_map>

class SoundBase;
class SourceVoice;

//サウンド(ソースボイス)生成クラス
class SoundCreater {
public:
    SoundCreater();
    ~SoundCreater();

    /// <summary>
    /// ソースボイスを生成する
    /// </summary>
    /// <param name="filePath">サウンドが置いてあるファイルパス</param>
    /// <param name="param">初期化用パラメータ</param>
    /// <returns>生成したソースボイス</returns>
    std::unique_ptr<SourceVoice> create(const std::string& filePath, const SourceVoiceInitParam& param);

private:
    SoundCreater(const SoundCreater&) = delete;
    SoundCreater& operator=(const SoundCreater&) = delete;

    //まだ読み込んだことがなければ、音を読み込む
    void loadSound(std::shared_ptr<WaveformData>* data, const std::string& filePath, const SourceVoiceInitParam& param);
    //拡張子を識別し、パーサーを生成する
    std::shared_ptr<ISoundLoader> createLoaderFromFilePath(const std::string& filePath);

private:
    std::unique_ptr<SoundBase> mSoundBase;
    std::unordered_map<std::string, std::shared_ptr<WaveformData>> mSounds;

    static inline bool mInstantiated = false;
};
