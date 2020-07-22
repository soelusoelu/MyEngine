﻿#include "Title.h"
#include "Scene.h"
#include "../Sound/SoundComponent.h"
#include "../Sound/SubmixVoiceComponent.h"
#include "../../Device/AssetsManager.h"
#include "../../Device/SoundCreater.h"
#include "../../Input/Input.h"
#include "../../Sound/Data/SoundBuffer.h"
#include "../../Sound/Effects/Reverb.h"
#include "../../Sound/Effects/SoundEffect.h"
#include "../../Sound/Effects/SoundFilter.h"
#include "../../Sound/Output/OutputVoices.h"
#include "../../Sound/Player/Frequency.h"
#include "../../Sound/Player/SoundPlayer.h"
#include "../../Sound/Voice/IVoice.h"
#include "../../Sound/Voice/VoiceDetails.h"
#include "../../Sound/Voice/SourceVoice/SourceVoice.h"
#include "../../Sound/Voice/SubmixVoice/SubmixVoice.h"
#include "../../Sound/Voice/SubmixVoice/SubmixVoiceInitParam.h"
#include "../../Sound/Volume/SoundFade.h"
#include "../../Sound/Volume/SoundPan.h"
#include "../../Sound/Volume/SoundVolume.h"
#include "../../System/World.h"

Title::Title() :
    Component(),
    mScene(nullptr),
    mSound(nullptr),
    mWetSubmix(nullptr),
    mDrySubmix(nullptr) {
}

Title::~Title() = default;

void Title::awake() {
}

void Title::start() {
    mScene = getComponent<Scene>();
    mSound = getComponent<SoundComponent>();

    if (!mSound || mSound->isNull()) {
        return;
    }

    SubmixVoiceInitParam param;
    const auto& data = mSound->getVoiceDetails();
    param.inputChannels = data.inputChannels;
    param.inputSampleRate = data.samplesPerSec;
    auto& soundCreater = World::instance().assetsManager().getSoundCreater();
    mWetSubmix = soundCreater.createSubmixVoice(param);
    mDrySubmix = soundCreater.createSubmixVoice(param);

    if (!mWetSubmix || !mDrySubmix) {
        return;
    }

    mSound->getOutputVoices().addOutputVoice(mWetSubmix, false, false);
    mSound->getOutputVoices().addOutputVoice(mDrySubmix);

    //mSound->getSoundVolume().setVolume(0.f);
    //mSound->getSoundVolume().fade().settings(0.5f, 5.f);
    //mSound->getSoundPlayer().frequency().setFrequencyRatio(4.f);
    //mSound->getSoundBuffer().loopCount = 1;
    //mSound->getSoundBuffer().playBegin = 60.f;
    //mSound->getSoundBuffer().playLength = 30.f;
    //mSound->getSoundBuffer().loopBegin = 60.f;
    //mSound->getSoundBuffer().loopLength = 30.f;
    //mSound->getSoundFilter().lowPassFilter(2500.f);
    //mSound->getSoundFilter().highPassFilter(250.f);
    //mSound->getSoundFilter().bandPassFilter(1000.f);
    mSound->getSoundVolume().getSoundPan().pan(0.f);
    mSound->getSoundVolume().getSoundPan().panCenter();

    //サウンドエフェクト
    //int reverbID = mWetSubmix->getSoundEffect().reverb();
    int echoID = mWetSubmix->getSoundEffect().echo();
    mWetSubmix->getSoundEffect().apply();

    //auto reverbParam = Reverb::getParameters();
    //reverbParam.WetDryMix = 20.f;
    //mWetSubmix->getSoundEffect().setEffectParameters(reverbID, &reverbParam, sizeof(reverbParam));
    mWetSubmix->getSoundVolume().setVolume(0.5f);
    mDrySubmix->getSoundVolume().setVolume(0.5f);

    mSound->getSoundPlayer().playFadeIn(0.75f, 2.f);
}

void Title::update() {
    auto isEnd = Input::joyPad()->getEnter();
    if (!isEnd) {
        isEnd = Input::keyboard()->getEnter();
    }
    if (isEnd) {
        mScene->next("GamePlay");
    }

    if (Input::keyboard()->getKeyDown(KeyCode::Alpha1)) {
        mSound->getSoundPlayer().playFadeIn(1.f, 1.f);
    } else if (Input::keyboard()->getKeyDown(KeyCode::Alpha2)) {
        mSound->getSoundPlayer().pauseFadeOut(1.f);
    } else if (Input::keyboard()->getKeyDown(KeyCode::Alpha3)) {
        mSound->getSoundFilter().resetFilter();
    } else if (Input::keyboard()->getKeyDown(KeyCode::Alpha0)) {
        mSound->getSoundVolume().getSoundPan().pan(0.f);
    } else if (Input::keyboard()->getKeyDown(KeyCode::Alpha9)) {
        mSound->getSoundVolume().getSoundPan().panCenter();
    }
}
