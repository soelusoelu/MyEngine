#include "GamePlay.h"
#include "Scene.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"

GamePlay::GamePlay() :
    Component(),
    mScene(nullptr) {
}

GamePlay::~GamePlay() = default;

void GamePlay::start() {
    mScene = getComponent<Scene>();

    GameObjectCreater::create("Test");
}

void GamePlay::update() {
#ifdef _DEBUG
    //リセット
    if (Input::keyboard().getKeyDown(KeyCode::R)) {
        mScene->next("GamePlay");
    }
#endif // _DEBUG
}
