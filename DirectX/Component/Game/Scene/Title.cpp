#include "Title.h"
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
    GameObjectCreater::create("Player");
    GameObjectCreater::create("Plane");
}
