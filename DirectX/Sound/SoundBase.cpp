#include "SoundBase.h"
#include "SoundLoader.h"
#include "SourceVoice.h"
#include "SubmixVoice.h"
#include "../DebugLayer/Debug.h"
#include "../Device/Flag.h"
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

std::unique_ptr<SourceVoice> SoundBase::createSourceVoice(const SoundLoader& data, const SourceVoiceInitParam& param) const {
    IXAudio2SourceVoice* sourceVoice;
    auto res = mXAudio2->CreateSourceVoice(&sourceVoice, data.format(), param.flags.get(), param.maxFrequencyRatio, param.callback, param.sendList, param.effectChain);

    if (FAILED(res)) {
        Debug::logError("Failed created source voice.");
        return nullptr;
    }

    return std::make_unique<SourceVoice>(sourceVoice, data, param);
}

std::unique_ptr<SubmixVoice> SoundBase::createSubmixVoice(const SubmixVoiceInitParam& param) const {
    IXAudio2SubmixVoice* submixVoice;
    auto res = mXAudio2->CreateSubmixVoice(&submixVoice, param.inputChannels, param.inputSampleRate, param.flags.get(), param.processingStage, param.sendList, param.effectChain);

    if (FAILED(res)) {
        Debug::logError("Failed created submix voice.");
        return nullptr;
    }

    return std::make_unique<SubmixVoice>(submixVoice, param);
}

bool SoundBase::comInitialize() {
    if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
        Debug::windowMessage("サウンドCOMシステムの初期化に失敗しました");
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
