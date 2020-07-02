#include "Title.h"
#include "Scene.h"
#include "../Sound/SoundComponent.h"
#include "../../Input/Input.h"
#include "../../Sound/SoundPlayer.h"
#include "../../Sound/SoundVolume.h"
#include "../../Sound/SoundFade.h"

Title::Title() :
    Component(),
    mScene(nullptr),
    mSound(nullptr) {
}

Title::~Title() = default;

void Title::start() {
    mScene = getComponent<Scene>();
    mSound = getComponent<SoundComponent>();
    mSound->getSoundVolume().setVolume(0.f);
    mSound->getSoundVolume().fade().settings(1.f, 10.f);
    mSound->getSoundPlayer().play();
}

void Title::update() {
    auto isEnd = Input::joyPad()->getEnter();
    if (!isEnd) {
        isEnd = Input::keyboard()->getEnter();
    }

    mSound->getSoundVolume().fade().updateFade();
    if (isEnd) {
        //mScene->next("GamePlay");
        mSound->getSoundPlayer().play();
    }
}
