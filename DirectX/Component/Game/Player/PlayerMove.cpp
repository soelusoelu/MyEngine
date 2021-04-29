#include "PlayerMove.h"
#include "../../../Input/Input.h"
#include "../../../Transform/Transform3D.h"

PlayerMove::PlayerMove(GameObject& gameObject)
    : Component(gameObject)
{
}

PlayerMove::~PlayerMove() = default;

void PlayerMove::update() {
    auto& key = Input::keyboard();
    auto h = key.horizontal();
    auto v = key.vertical();

    auto& t = transform();
    if (h != 0 || v != 0) {
        t.translate(t.forward() * Vector3(h, 0.f, v));
    }
}
