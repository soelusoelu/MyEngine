﻿#pragma once

#include <rapidjson/document.h>
#include <memory>
#include <string>
#include <unordered_set>

class Scene;
class Camera;
class DirectionalLight;
class GameObjectManager;
class MeshManager;
class Physics;
class Renderer;
class SpriteManager;
class LightManager;

class SceneManager {
    using StringSet = std::unordered_set<std::string>;

public:
    SceneManager(const std::shared_ptr<Renderer>& renderer);
    ~SceneManager();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    void update();
    void draw() const;

private:
    void change(const StringSet& tags);
    void createScene(const std::string& name);

private:
    std::shared_ptr<Renderer> mRenderer;
    std::shared_ptr<Scene> mCurrentScene;
    std::shared_ptr<Camera> mCamera;
    GameObjectManager* mGameObjectManager;
    MeshManager* mMeshManager;
    SpriteManager* mSpriteManager;
    Physics* mPhysics;
    LightManager* mLightManager;
    bool mShouldDraw;
};
