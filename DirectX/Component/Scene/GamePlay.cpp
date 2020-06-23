#include "GamePlay.h"
#include "Scene.h"
#include "../../Component/ComponentManager.h"
#include "../../GameObject/GameObject.h"
#include "../../Input/Input.h"

GamePlay::GamePlay() :
    Component(),
    mScene(nullptr) {
}

GamePlay::~GamePlay() = default;

void GamePlay::start() {
    mScene = gameObject()->componentManager()->getComponent<Scene>();
}

void GamePlay::update() {
#ifdef _DEBUG
    //リセット
    if (Input::keyboard()->getKeyDown(KeyCode::R)) {
        mScene->next("GamePlay");
    }
#endif // _DEBUG
}
