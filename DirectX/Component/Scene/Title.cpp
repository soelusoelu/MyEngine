#include "Title.h"
#include "Scene.h"
#include "../Sound/SoundComponent.h"
#include "../../Input/Input.h"
#include "../../Sound/Frequency.h"
#include "../../Sound/SoundBuffer.h"
#include "../../Sound/SoundFade.h"
#include "../../Sound/SoundFilter.h"
#include "../../Sound/SoundPlayer.h"
#include "../../Sound/SoundVolume.h"

Title::Title() :
    Component(),
    mScene(nullptr),
    mSound(nullptr) {
}

Title::~Title() = default;

void Title::start() {
    mScene = getComponent<Scene>();
    mSound = getComponent<SoundComponent>();
    //mSound->getSoundVolume().setVolume(0.f);
    //mSound->getSoundVolume().fade().settings(0.5f, 5.f);
    //mSound->getSoundVolume().frequency().setFrequencyRatio(4.f);
    //mSound->getSoundBuffer().loopCount = 1;
    //mSound->getSoundBuffer().playBegin = 60.f;
    //mSound->getSoundBuffer().playLength = 30.f;
    //mSound->getSoundBuffer().loopBegin = 60.f;
    //mSound->getSoundBuffer().loopLength = 30.f;
    //mSound->getSoundFilter().lowPassFilter(2500.f);
    //mSound->getSoundFilter().highPassFilter(250.f);
    //mSound->getSoundFilter().bandPassFilter(1000.f);
    //float volumes[2][2] = {
    //    { 1.f, 1.f }, //左スピーカー
    //    { 0.f, 0.f }  //右スピーカー
    //};
    //mSound->getSoundVolume().pan(*volumes);
    //float volumes[] = { 1.f, 1.f, 0.f, 0.f };
    //mSound->getSoundVolume().pan(volumes);
    mSound->getSoundPlayer().playFadeIn(1.f, 2.f);
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
    }
}
