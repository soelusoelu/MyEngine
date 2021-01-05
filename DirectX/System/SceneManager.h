#pragma once

#include "FpsCounter/IFpsGetter.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>
#include <unordered_set>

class Scene;
class Renderer;
class Camera;
class EngineFunctionManager;
class GameObjectManager;
class MeshManager;
class Physics;
class SpriteManager;
class LightManager;
class DrawString;

class SceneManager {
public:
    SceneManager();
    ~SceneManager();
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
    void initialize(const IFpsGetter* fpsGetter);
    void update();
    void draw() const;

private:
    void change();
    void createScene(const std::string& name);

private:
    std::unique_ptr<Renderer> mRenderer;
    std::shared_ptr<Scene> mCurrentScene;
    std::shared_ptr<Camera> mCamera;
    std::unique_ptr<EngineFunctionManager> mEngineManager;
    std::unique_ptr<GameObjectManager> mGameObjectManager;
    std::unique_ptr<MeshManager> mMeshManager;
    std::unique_ptr<SpriteManager> mSpriteManager;
    std::unique_ptr<Physics> mPhysics;
    std::unique_ptr<LightManager> mLightManager;
    DrawString* mTextDrawer;
    std::string mBeginScene;
    std::unordered_set<std::string> mRemoveExclusionTags;
};
