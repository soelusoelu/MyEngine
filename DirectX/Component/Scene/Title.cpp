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
    mSound->getSoundVolume().setVolume(0.5f);
    //mSound->getSoundVolume().fade().settings(1.f, 10.f);
    float volumes[] = { 1.f, 0.f };
    mSound->getSoundVolume().panning(2, volumes);
    //mSound->getSoundPlayer().play();
}

void Title::update() {
    auto isEnd = Input::joyPad()->getEnter();
    if (!isEnd) {
        isEnd = Input::keyboard()->getEnter();
    }
    if (isEnd) {
        mScene->next("GamePlay");
    }

    //mSound->getSoundVolume().fade().updateFade();
    if (Input::keyboard()->getKeyDown(KeyCode::Alpha1)) {
        mSound->getSoundPlayer().play();
    }
}
