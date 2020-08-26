#include "ListenerComponent.h"
#include "../../Sound/3D/Listener/Sound3DListener.h"
#include "../../Transform/Transform3D.h"

ListenerComponent::ListenerComponent(GameObject& gameObject) :
    Component(gameObject),
    mListener(std::make_unique<Sound3DListener>()) {
}

ListenerComponent::~ListenerComponent() = default;

void ListenerComponent::lateUpdate() {
    mListener->setFrontAndTop(transform().forward(), transform().up());
    mListener->setPosition(transform().getPosition());
    mListener->update();
}

void ListenerComponent::setCone(Sound3DCone* cone) {
    mListener->setCone(cone);
}

const Sound3DListenerParam& ListenerComponent::getListener() const {
    return mListener->getListener();
}
