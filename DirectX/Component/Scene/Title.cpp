#include "Title.h"
#include "Scene.h"
#include "../Sound/SoundComponent.h"
#include "../../Input/Input.h"
#include "../../Math/Math.h"
#include "../../Device/Time.h"

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

    static float volume = 0.f;
    volume += Time::deltaTime * 60.f;
    mSound->setVolume(Math::abs(Math::cos(volume)));
    if (isEnd) {
        //mScene->next("GamePlay");
        mSound->play();
    }
}
