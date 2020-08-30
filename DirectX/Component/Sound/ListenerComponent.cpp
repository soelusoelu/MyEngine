﻿#include "ListenerComponent.h"
#include "../../Sound/3D/Listener/Sound3DListener.h"
#include "../../Transform/Transform3D.h"

ListenerComponent::ListenerComponent(GameObject& gameObject) :
    Component(gameObject),
    mListener(std::make_shared<Sound3DListener>()) {
}

ListenerComponent::~ListenerComponent() = default;

void ListenerComponent::onUpdateWorldTransform() {
    mListener->setFrontAndTop(transform().forward(), transform().up());
    mListener->setPosition(transform().getPosition());
}

void ListenerComponent::setCone(Sound3DCone* cone) {
    mListener->setCone(cone);
}

std::shared_ptr<Sound3DListener> ListenerComponent::getListener() const {
    return mListener;
}

const Sound3DListenerParam& ListenerComponent::getListenerParam() const {
    return mListener->getListener();
}
