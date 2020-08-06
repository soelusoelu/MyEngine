#pragma once

class SoundStreaming {
public:
    SoundStreaming();
    ~SoundStreaming();

private:
    SoundStreaming(const SoundStreaming&) = delete;
    SoundStreaming& operator=(const SoundStreaming&) = delete;

private:
    //どこまで読み込んだか
    unsigned mWrite;
};
