#include "Title.h"
#include "Scene.h"
#include "../ComponentManager.h"
#include "../../GameObject/GameObject.h"
#include "../../Input/Input.h"

Title::Title() :
    Component(),
    mScene(nullptr) {
}

Title::~Title() = default;

void Title::start() {
    mScene = gameObject()->componentManager()->getComponent<Scene>();
}

void Title::update() {
    auto isEnd = Input::joyPad()->getEnter();
#ifdef _DEBUG
    if (!isEnd) {
        isEnd = Input::keyboard()->getEnter();
    }
#endif // _DEBUG

    if (isEnd) {
        mScene->next("GamePlay");
    }
}
