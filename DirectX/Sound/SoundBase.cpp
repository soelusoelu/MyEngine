#include "SoundBase.h"
#include "SoundLoader.h"
#include "SourceVoice.h"
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
    mMasteringVoice = nullptr;
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

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::windowMessage("ソースボイス作成失敗");
    }
#endif // _DEBUG
}

std::shared_ptr<SourceVoice> SoundBase::createSourceVoice(const std::shared_ptr<SoundLoader>& data) const {
    IXAudio2SourceVoice* sourceVoice;
    auto res = mXAudio2->CreateSourceVoice(&sourceVoice, data->format());

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::windowMessage("ソースボイス作成失敗");
        return nullptr;
    }
#endif // _DEBUG

    return std::make_shared<SourceVoice>(sourceVoice, data);
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
