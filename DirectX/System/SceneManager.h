#pragma once

#include "FpsCounter/IFpsGetter.h"
#include "../Device/FileOperator.h"
#include "../Engine/EngineMode.h"
#include "../Engine/IEngineModeGetter.h"
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
class MeshRenderOnTextureManager;
class DrawString;

class SceneManager
    : public FileOperator
    , public IEngineModeGetter
{
public:
    SceneManager();
    ~SceneManager();
    void initialize(const IFpsGetter* fpsGetter);
    void update();
    void draw() const;

private:
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    virtual EngineMode getMode() const override;

    //シーン変更時
    void change();
    //シーン生成
    void createScene(const std::string& name);
    //最初のシーンを選択する
    void choiceBeginScene();
    //ゲーム中か
    bool isGameMode() const;
    //モード変更時
    void onChangeMode(EngineMode mode);

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
    std::unique_ptr<MeshRenderOnTextureManager> mMeshRenderOnTextureManager;
    DrawString* mTextDrawer;
    std::string mBeginScene;
    std::string mReleaseScene;
    std::unordered_set<std::string> mRemoveExclusionTags;
    EngineMode mMode;
};
