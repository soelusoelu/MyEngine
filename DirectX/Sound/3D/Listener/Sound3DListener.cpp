#include "Sound3DListener.h"
#include "../../../Component/Camera/Camera.h"
#include "../../../Device/Time.h"
#include "../../../Transform/Transform3D.h"

Sound3DListener::Sound3DListener() :
    mListener(),
    mPreviousPos(Vector3::zero) {
    mListener.cone = &LISTENER_DIRECTIONAL_CONE;
}

Sound3DListener::~Sound3DListener() = default;

void Sound3DListener::update() {
    mListener.velocity = (mListener.position - mPreviousPos) / Time::deltaTime;
    mPreviousPos = mListener.position;
}

void Sound3DListener::setListenerByCamera(const Camera& camera) {
    mListener.orientFront = camera.transform().forward();
    mListener.orientTop = camera.transform().up();
    mListener.position = camera.getPosition();
}

void Sound3DListener::setCone(Sound3DCone* cone) {
    mListener.cone = cone;
}

const Sound3DListenerParam& Sound3DListener::getListener() const {
    return mListener;
}
