#include "Sound.h"
#include "SoundLoader.h"
#include "../DebugLayer/Debug.h"

Sound::Sound() :
    mSourceVoice(nullptr),
    mData(nullptr) {
}

Sound::~Sound() {
    mSourceVoice->DestroyVoice();
    mSourceVoice = nullptr;
}

void Sound::play(bool isLoop) {
    XAUDIO2_BUFFER buffer = { 0 };
    buffer.pAudioData = mData->buffer();
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes = mData->size();
    if (isLoop) {
        buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    }
    if (FAILED(mSourceVoice->SubmitSourceBuffer(&buffer))) {
        Debug::windowMessage("ソースボイスにサブミット失敗");
        return;
    }
    mSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
}

void Sound::stop() {
    mSourceVoice->Stop(0, XAUDIO2_COMMIT_NOW);
}

bool Sound::isFinished() const {
    XAUDIO2_VOICE_STATE state;
    mSourceVoice->GetState(&state);
    return (state.BuffersQueued == 0);
}

void Sound::setVolume(float volume) {
    mSourceVoice->SetVolume(volume);
}
