#include "SoundBase.h"
#include "Sound.h"
#include "SoundLoader.h"
#include "../DebugLayer/Debug.h"
#include "../System/GlobalFunction.h"
#include <cassert>

SoundBase::SoundBase() :
    mXAudio2(nullptr),
    mMasteringVoice(nullptr),
    mSucceedInitialize(false) {

    //インスタンスを1つに制限
    assert(!mInstantiated);
    mInstantiated = true;

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

    mInstantiated = false;
}

void SoundBase::createSourceVoice(
    IXAudio2SourceVoice** sourceVoice,
    const WAVEFORMATEX* sourceFormat,
    unsigned flags,
    float maxFrequencyRatio,
    IXAudio2VoiceCallback* callback,
    const XAUDIO2_VOICE_SENDS* sendList,
    const XAUDIO2_EFFECT_CHAIN* effectChain
) const {
    auto res = mXAudio2->CreateSourceVoice(sourceVoice, sourceFormat, flags, maxFrequencyRatio, callback, sendList, effectChain);
    if (FAILED(res)) {
        Debug::windowMessage("ソースボイス作成失敗");
    }
}

void SoundBase::createSourceVoice(std::shared_ptr<Sound>* sound, const std::shared_ptr<SoundLoader>& data) const {
    IXAudio2SourceVoice* sourceVoice;
    auto res = mXAudio2->CreateSourceVoice(&sourceVoice, data->format());
    if (FAILED(res)) {
        Debug::windowMessage("ソースボイス作成失敗");
        return;
    }

    //既に生成済みなら破棄・初期化
    if ((*sound)->mSourceVoice) {
        (*sound)->mSourceVoice->DestroyVoice();
        (*sound)->mSourceVoice = nullptr;
    }

    (*sound)->mSourceVoice = sourceVoice;
    (*sound)->mData = data;
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
