﻿#include "DirectionalLight.h"
#include "../../GameObject/Transform3D.h"
#include "../../Utility/LevelLoader.h"

DirectionalLight::DirectionalLight() :
    Component(),
    mDirection(Vector3::zero),
    mLightColor(Vector3::one) {
}

DirectionalLight::~DirectionalLight() = default;

void DirectionalLight::onUpdateWorldTransform() {
    mDirection = Vector3::transform(Vector3::up, transform().getRotation());
}

void DirectionalLight::loadProperties(const rapidjson::Value& inObj) {
    if (JsonHelper::getVector3(inObj, "direction", &mDirection)) {
        transform().rotate(mDirection);
    }
    JsonHelper::getVector3(inObj, "color", &mLightColor);
}

void DirectionalLight::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("Color", mLightColor);
}

void DirectionalLight::setDirection(const Vector3& dir) {
    mDirection = dir;
}

const Vector3& DirectionalLight::getDirection() const {
    return mDirection;
}

void DirectionalLight::setLightColor(const Vector3& color) {
    mLightColor = color;
}

const Vector3& DirectionalLight::getLightColor() const {
    return mLightColor;
}
