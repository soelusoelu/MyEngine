﻿#include "Scene.h"

Scene::Scene()
    : Component()
    , mNext()
{
}

Scene::~Scene() = default;

void Scene::next(const std::string& next) {
    mNext = next;
}

const std::string& Scene::getNext() const {
    return mNext;
}

void Scene::addValuePassToNextScene(const std::string& valueName, const std::any& value) {
    mValuesPassToNextScene.emplace(valueName, value);
}

const ValuePassMap& Scene::getValuePassToNextScene() const {
    return mValuesPassToNextScene;
}
