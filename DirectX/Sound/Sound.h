#pragma once

#include <xaudio2.h>
#include <memory>

class SoundLoader;

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
    std::shared_ptr<SoundLoader> mData;
};
