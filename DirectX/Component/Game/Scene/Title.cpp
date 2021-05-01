#include "Title.h"
#include "../Camera/GameCamera.h"
#include "../../Engine/Mesh/MeshComponent.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectFactory.h"
#include "../../../Transform/Transform3D.h"

Title::Title(GameObject& gameObject) :
    Scene(gameObject)
{
}

Title::~Title() = default;

void Title::awake() {
    auto player = GameObjectCreater::create("Player");
    GameObjectCreater::create("Plane");
    auto camera = GameObjectCreater::create("GameCamera");
    camera->componentManager().getComponent<GameCamera>()->setPlayer(player);
}
