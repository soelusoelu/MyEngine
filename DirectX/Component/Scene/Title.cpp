#include "Title.h"
#include "Scene.h"
#include "../../Input/Input.h"

Title::Title() :
    Component(),
    mScene(nullptr) {
}

Title::~Title() = default;

void Title::start() {
    mScene = getComponent<Scene>();
}

void Title::update() {
    auto isEnd = Input::joyPad()->getEnter();
    if (!isEnd) {
        isEnd = Input::keyboard()->getEnter();
    }

    if (isEnd) {
        mScene->next("GamePlay");
    }
}
