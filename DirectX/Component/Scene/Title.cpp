#include "Title.h"
#include "Scene.h"
#include "../Sound/SoundComponent.h"
#include "../../Input/Input.h"

Title::Title() :
    Component(),
    mScene(nullptr),
    mSound(nullptr) {
}

Title::~Title() = default;

void Title::start() {
    mScene = getComponent<Scene>();
    mSound = getComponent<SoundComponent>();
}

void Title::update() {
    auto isEnd = Input::joyPad()->getEnter();
    if (!isEnd) {
        isEnd = Input::keyboard()->getEnter();
    }

    if (isEnd) {
        //mScene->next("GamePlay");
        //mSound->playBGM();
    }
}
