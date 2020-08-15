#include "ListenerComponent.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../Sound/3D/Listener/Sound3DListener.h"
#include "../../Transform/Transform3D.h"

ListenerComponent::ListenerComponent() :
    Component(),
    mListener(std::make_unique<Sound3DListener>()),
    mTarget(nullptr) {
}

ListenerComponent::~ListenerComponent() = default;

void ListenerComponent::lateUpdate() {
    if (!mTarget) {
        Debug::logWarning("The listener target is not set.");
        return;
    }
    auto& transform = mTarget->transform();
    mListener->setFrontAndTop(transform.forward(), transform.up());
    mListener->setPosition(transform.getPosition());
    mListener->update();
}

bool ListenerComponent::isNull() const {
    return (mTarget) ? false : true;
}

void ListenerComponent::setTarget(const std::shared_ptr<GameObject>& target) {
    mTarget = target;
}

void ListenerComponent::setCone(Sound3DCone* cone) {
    mListener->setCone(cone);
}

const Sound3DListenerParam& ListenerComponent::getListener() const {
    return mListener->getListener();
}
