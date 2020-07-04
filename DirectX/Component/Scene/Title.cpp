#include "Title.h"
#include "Scene.h"
#include "../Sound/SoundComponent.h"
#include "../../Input/Input.h"
#include "../../Sound/Frequency.h"
#include "../../Sound/SoundBuffer.h"
#include "../../Sound/SoundFade.h"
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
        mSound->getSoundPlayer().play();
    } else if (Input::keyboard()->getKeyDown(KeyCode::Alpha2)) {
        mSound->getSoundPlayer().pauseFadeOut(1.f);
    }
}
