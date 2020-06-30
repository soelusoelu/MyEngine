#pragma once

#include <XAudio2.h>
#include <memory>
#include <string>

class Sound;

class SoundBase {
public:
    SoundBase();
    ~SoundBase();
    void load(const std::string& fileName, std::shared_ptr<Sound>* sound);
    void createSourceVoice(std::shared_ptr<Sound>* sound);

private:
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
};

class Sound {
    friend class SoundBase;

public:
    Sound();
    ~Sound();
    void play(bool isLoop = false);
    void stop();
    bool isFinished() const;
    void setVolume(float volume);

private:
    IXAudio2SourceVoice* mSourceVoice;
    BYTE* mWavBuffer; //波形データ(フォーマット等を含まない、純粋に波形データのみ)
    unsigned mWavSize; //波形データのサイズ
    WAVEFORMATEX* mWavFormat;
};
