#include "Sound.h"
#include "../DebugLayer/Debug.h"
#include "../System/GlobalFunction.h"
#include "../System/World.h"
#include "../Utility/Directory.h"

SoundBase::SoundBase() :
    mXAudio2(nullptr),
    mMasteringVoice(nullptr),
    mSucceedInitialize(false) {

    mSucceedInitialize = comInitialize();
    if (!mSucceedInitialize) {
        return;
    }

    mSucceedInitialize = createXAudio2();
    if (!mSucceedInitialize) {
        return;
    }

    mSucceedInitialize = createMasteringVoice();
}

SoundBase::~SoundBase() {
    mMasteringVoice->DestroyVoice();
    safeRelease(mXAudio2);
    CoUninitialize();
}

void SoundBase::load(const std::string& fileName, std::shared_ptr<Sound>* sound) {
    World::instance().directory().setSoundDirectory();

    HMMIO hMmio = NULL; //WindowsマルチメディアAPIのハンドル(WindowsマルチメディアAPIはWAVファイル関係の操作用のAPI)
    unsigned wavSize = 0; //WAVファイル内 WAVデータのサイズ(WAVファイルはWAVデータで占められているので、ほぼファイルサイズと同一)
    WAVEFORMATEX* pwfex; //WAVのフォーマット 例)16ビット、44100Hz、ステレオなど
    MMCKINFO ckInfo; //チャンク情報
    MMCKINFO riffckInfo; // 最上部チャンク(RIFFチャンク)保存用
    PCMWAVEFORMAT pcmWaveForm;
    //WAVファイル内のヘッダー情報(音データ以外)の確認と読み込み
    char* fn = const_cast<char*>(fileName.c_str()); //他にやり方思い浮かばない
    hMmio = mmioOpenA(fn, NULL, MMIO_ALLOCBUF | MMIO_READ);
    //ファイルポインタをRIFFチャンクの先頭にセットする
    mmioDescend(hMmio, &riffckInfo, NULL, 0);
    //ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
    ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
    mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);
    //フォーマットを読み込む
    mmioRead(hMmio, reinterpret_cast<HPSTR>(&pcmWaveForm), sizeof(pcmWaveForm));
    pwfex = reinterpret_cast<WAVEFORMATEX*>(new CHAR[sizeof(WAVEFORMATEX)]);
    memcpy(pwfex, &pcmWaveForm, sizeof(pcmWaveForm));
    pwfex->cbSize = 0;
    mmioAscend(hMmio, &ckInfo, 0);
    //WAVファイル内の音データの読み込み
    ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
    mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK); //データチャンクにセット
    wavSize = ckInfo.cksize;
    (*sound)->mWavBuffer = new BYTE[wavSize];
    DWORD offset = ckInfo.dwDataOffset;
    mmioRead(hMmio, reinterpret_cast<HPSTR>((*sound)->mWavBuffer), wavSize);
    //ソースボイスにデータを詰め込む
    if (FAILED(mXAudio2->CreateSourceVoice(&(*sound)->mSourceVoice, pwfex))) {
        Debug::windowMessage("ソースボイス作成失敗");
    }
    (*sound)->mWavSize = wavSize;
    (*sound)->mWavFormat = pwfex;
}

void SoundBase::createSourceVoice(std::shared_ptr<Sound>* sound) {
    //ソースボイスにデータを詰め込む
    if (FAILED(mXAudio2->CreateSourceVoice(&(*sound)->mSourceVoice, (*sound)->mWavFormat))) {
        Debug::windowMessage("ソースボイス作成失敗");
    }
}

bool SoundBase::comInitialize() {
    if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
        Debug::windowMessage("COMシステムの初期化に失敗しました");
        return false;
    }
    return true;
}

bool SoundBase::createXAudio2() {
    if (FAILED(XAudio2Create(&mXAudio2, 0))) {
        Debug::windowMessage("XAudio2の生成に失敗しました");
        CoUninitialize();
        return false;
    }

    //デバッグ指定
#ifdef _DEBUG
    XAUDIO2_DEBUG_CONFIGURATION debug = { 0 };
    debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
    debug.BreakMask = XAUDIO2_LOG_ERRORS;
    mXAudio2->SetDebugConfiguration(&debug, 0);
#endif // _DEBUG

    return true;
}

bool SoundBase::createMasteringVoice() {
    if (FAILED(mXAudio2->CreateMasteringVoice(&mMasteringVoice))) {
        Debug::windowMessage("MasteringVoiceの生成に失敗しました");
        CoUninitialize();
        return false;
    }
    return true;
}



Sound::Sound() :
    mSourceVoice(nullptr),
    mWavBuffer(nullptr),
    mWavSize(0),
    mWavFormat(nullptr) {
}

Sound::~Sound() {
    mSourceVoice->DestroyVoice();
    safeDelete(mWavBuffer);
    safeDelete(mWavFormat);
}

void Sound::play(bool isLoop) {
    XAUDIO2_BUFFER buffer = { 0 };
    buffer.pAudioData = mWavBuffer;
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes = mWavSize;
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
