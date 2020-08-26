#include "GamePlay.h"
#include "Scene.h"
#include "../Sound/ListenerComponent.h"
#include "../Sound/SoundComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Input/Input.h"

GamePlay::GamePlay() :
    Component(),
    mScene(nullptr) {
}

GamePlay::~GamePlay() = default;

void GamePlay::start() {
    mScene = getComponent<Scene>();

    //エミッターを生成
    auto emitterObj = GameObjectCreater::create("Test");
    auto emitter = emitterObj->componentManager().getComponent<SoundComponent>();
    auto emitterObj2 = GameObjectCreater::create("Test2");
    auto emitter2 = emitterObj2->componentManager().getComponent<SoundComponent>();
    //リスナーに設定するために
    auto camera = gameObject()->getGameObjectManager().find("Camera");
    //リスナーオブジェクトを生成
    auto listenerObj = GameObjectCreater::create("Listener");
    //リスナーコンポーネントを取得
    auto listener = listenerObj->componentManager().getComponent<ListenerComponent>();
    //カメラをリスナーとして設定
    listener->setTarget(camera);
    //エミッターにリスナーを設定
    //emitter->setListener(listener);
    //emitter2->setListener(listener);
}

void GamePlay::update() {
#ifdef _DEBUG
    //リセット
    if (Input::keyboard().getKeyDown(KeyCode::R)) {
        mScene->next("GamePlay");
    }
#endif // _DEBUG
}
