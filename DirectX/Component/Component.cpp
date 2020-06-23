#include "Component.h"
#include "../GameObject/GameObject.h"

Component::Component() :
    mGameObject(),
    mComponentName("") {
}

Component::~Component() = default;

std::shared_ptr<GameObject> Component::gameObject() const {
    return mGameObject.lock();
}

const std::shared_ptr<Transform3D>& Component::transform() const {
    return gameObject()->transform();
}

const std::string& Component::getComponentName() const {
    return mComponentName;
}
