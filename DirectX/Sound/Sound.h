#pragma once

#define XAUDIO2_HELPER_FUNCTIONS
#include <xaudio2.h>
#include <memory>

class SourceVoice;
class SoundLoader;

//サウンド関連ライブラリ
class Sound {
    friend class SoundBase;

public:
    Sound();
    ~Sound();
    void play(bool isLoop = false);
    void stop() const;
    //再生が終了しているか
    bool isFinished() const;
    //音量を変更する
    void setVolume(float volume) const;
    //デシベルで音量を変更する
    void setVolumeByDecibels(float decibels) const;
    //ボリューム値からデシベル値に変換する
    float amplitudeToDecibelsRatio(float volume) const;
    //デシベル値からボリューム値に変換する
    float decibelsToAmplitudeRatio(float decibels) const;

private:
    std::shared_ptr<SourceVoice> mSourceVoice;
    std::shared_ptr<SoundLoader> mData;
};
