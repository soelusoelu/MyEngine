#include "Title.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectFactory.h"

Title::Title(GameObject& gameObject) :
    Scene(gameObject)
{
}

Title::~Title() = default;

void Title::awake() {
}
