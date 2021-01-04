#include "SceneManager.h"
#include "Game.h"
#include "GlobalFunction.h"
#include "../Component/ComponentManager.h"
#include "../Component/Engine/Camera/Camera.h"
#include "../Component/Engine/Scene/Scene.h"
#include "../Component/Engine/Text/TextBase.h"
#include "../Device/DrawString.h"
#include "../Device/Physics.h"
#include "../Device/Renderer.h"
#include "../Engine/EngineFunctionManager.h"
#include "../Engine/Debug/DebugUtility.h"
#include "../Engine/Debug/ImGuiInspector.h"
#include "../Engine/Pause/Pause.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../GameObject/GameObjectManager.h"
#include "../Light/LightManager.h"
#include "../Mesh/MeshManager.h"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteManager.h"
#include "../Utility/LevelLoader.h"
#include <vector>

SceneManager::SceneManager()
    : mRenderer(std::make_unique<Renderer>())
    , mCurrentScene(nullptr)
    , mCamera(nullptr)
    , mEngineManager(std::make_unique<EngineFunctionManager>())
    , mGameObjectManager(std::make_unique<GameObjectManager>())
    , mMeshManager(std::make_unique<MeshManager>())
    , mSpriteManager(std::make_unique<SpriteManager>())
    , mPhysics(std::make_unique<Physics>())
    , mLightManager(std::make_unique<LightManager>())
    , mTextDrawer(new DrawString())
    , mBeginScene()
{
}

SceneManager::~SceneManager() {
    safeDelete(mTextDrawer);

    TextBase::setDrawString(nullptr);
}

void SceneManager::loadProperties(const rapidjson::Value& inObj) {
    const auto& sceneObj = inObj["sceneManager"];
    if (sceneObj.IsObject()) {
        JsonHelper::getString(sceneObj, "beginScene", &mBeginScene);
        JsonHelper::getStringArray(sceneObj, "removeExclusionTag", &mRemoveExclusionTags);
    }

    mEngineManager->loadProperties(inObj);
    mLightManager->loadProperties(inObj);
    mTextDrawer->loadProperties(inObj);
}

void SceneManager::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    rapidjson::Value props(rapidjson::kObjectType);
    JsonHelper::setString(alloc, &props, "beginScene", mBeginScene);
    JsonHelper::setStringArray(alloc, &props, "removeExclusionTag", mRemoveExclusionTags);
    inObj.AddMember("sceneManager", props, alloc);

    mEngineManager->saveProperties(alloc, inObj);
    mLightManager->saveProperties(alloc, inObj);
    mTextDrawer->saveProperties(alloc, inObj);
}

void SceneManager::initialize() {
    mRenderer->initialize();
    mEngineManager->initialize(mGameObjectManager.get());
    mLightManager->initialize();
    mTextDrawer->initialize();

    TextBase::setDrawString(mTextDrawer);

    auto cam = GameObjectCreater::create("Camera");
    mCamera = cam->componentManager().getComponent<Camera>();
    //デフォルトのインスペクターの対象に設定する
    mEngineManager->debug().inspector().setTarget(cam);

    mMeshManager->createShadowMap();

    mLightManager->createDirectionalLight();

    mEngineManager->afterInitialize(mCamera, mMeshManager.get());

    //初期シーンの設定
    createScene(mBeginScene);
}

void SceneManager::update() {
    //アップデート前処理
    mEngineManager->preUpdateProcess();
    //デバッグ
    mEngineManager->update();

    //ポーズ中はデバッグだけアップデートを行う
    if (mEngineManager->pause().isPausing()) {
        return;
    }

    //保有しているテキストを全削除
    mTextDrawer->clear();
    //全ゲームオブジェクトの更新
    mGameObjectManager->update();
    //総当たり判定
    mPhysics->sweepAndPrune();
    //各マネージャークラスを更新
    mMeshManager->update();
    mSpriteManager->update();

    //シーン移行
    const auto& next = mCurrentScene->getNext();
    if (!next.empty()) {
        change();
        //次のシーンに渡す値を避難させとく
        const auto& toNextValues = mCurrentScene->getValuePassToNextScene();
        //シーン遷移
        createScene(next);
        //新しいシーンに前のシーンの値を渡す
        mCurrentScene->getValueFromPreviousScene(toNextValues);
    }
}

void SceneManager::draw() const {
#pragma region 遅延シェーディング
    //各テクスチャ上にレンダリング
    //mRenderer->renderToTexture();
    ////メッシュ描画準備
    //mRenderer->renderMesh();
    ////メッシュの一括描画
    //mMeshManager->draw(*mCamera, mLightManager->getDirectionalLight());
    ////各テクスチャを参照してレンダリング
    //mRenderer->renderFromTexture(*mCamera, *mLightManager);
    ////ポイントライト描画準備
    //mRenderer->renderPointLight();
    ////ポイントライトの一括描画
    //mLightManager->drawPointLights(*mCamera);
#pragma endregion

    //メッシュ描画準備
    mRenderer->renderMesh();
    //メッシュの描画
    mMeshManager->draw(*mCamera, mLightManager->getDirectionalLight());

#ifdef _DEBUG
    mEngineManager->draw3D(*mRenderer, mCamera->getViewProjection());
#endif // _DEBUG

    //スプライト描画準備
    mRenderer->renderSprite();
    //3Dスプライト
    mRenderer->renderSprite3D();
    mSpriteManager->draw3Ds(mCamera->getView(), mCamera->getProjection());
    //2Dスプライト
    auto proj = Matrix4::identity;
    mRenderer->renderSprite2D(proj);
    mSpriteManager->drawComponents(proj);

    //テキスト一括描画
    mTextDrawer->drawAll(proj);

#ifdef _DEBUG
    mEngineManager->draw(*mRenderer, proj);
#endif // _DEBUG
}

void SceneManager::change() {
    mGameObjectManager->clear(mRemoveExclusionTags);
    mMeshManager->clear();
    mSpriteManager->clear();
}

void SceneManager::createScene(const std::string& name) {
    //シーン作成
    auto scene = GameObjectCreater::create(name);
    //シーンコンポーネント取得
    mCurrentScene = scene->componentManager().getComponent<Scene>();

    //デバッグモード変更
    mEngineManager->changeScene(name);
}
