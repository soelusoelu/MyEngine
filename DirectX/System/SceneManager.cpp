#include "SceneManager.h"
#include "Game.h"
#include "GlobalFunction.h"
#include "../Component/ComponentManager.h"
#include "../Component/Camera/Camera.h"
#include "../Component/Collider/Collider.h"
#include "../Component/Light/PointLightComponent.h"
#include "../Component/Mesh/MeshComponent.h"
#include "../Component/Scene/Scene.h"
#include "../Component/Sprite/Sprite3D.h"
#include "../Component/Sprite/SpriteComponent.h"
#include "../Component/Text/TextBase.h"
#include "../DebugLayer/DebugUtility.h"
#include "../DebugLayer/Pause.h"
#include "../Device/DrawString.h"
#include "../Device/Physics.h"
#include "../Device/Renderer.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../GameObject/GameObjectManager.h"
#include "../Input/Input.h"
#include "../Input/Keyboard.h"
#include "../Light/LightManager.h"
#include "../Mesh/MeshManager.h"
#include "../Sprite/Sprite.h"
#include "../Sprite/SpriteManager.h"

SceneManager::SceneManager() :
    mRenderer(std::make_unique<Renderer>()),
    mCurrentScene(nullptr),
    mCamera(nullptr),
    mGameObjectManager(new GameObjectManager()),
    mMeshManager(new MeshManager()),
    mSpriteManager(new SpriteManager()),
    mPhysics(new Physics()),
    mLightManager(new LightManager()),
    mTextDrawer(new DrawString()),
    mShouldDraw(true) {
}

SceneManager::~SceneManager() {
    safeDelete(mGameObjectManager);
    safeDelete(mMeshManager);
    safeDelete(mSpriteManager);
    safeDelete(mPhysics);
    safeDelete(mLightManager);
    safeDelete(mTextDrawer);

    GameObject::setGameObjectManager(nullptr);
    MeshComponent::setMeshManager(nullptr);
    SpriteComponent::setSpriteManager(nullptr);
    Sprite3D::setSpriteManager(nullptr);
    Collider::setPhysics(nullptr);
    PointLightComponent::setLightManager(nullptr);
    TextBase::setDrawString(nullptr);
}

void SceneManager::loadProperties(const rapidjson::Value& inObj) {
    mLightManager->loadProperties(inObj);
    mTextDrawer->loadProperties(inObj);
}

void SceneManager::initialize() {
    mRenderer->initialize();
    mLightManager->initialize();
    mTextDrawer->initialize();

    GameObject::setGameObjectManager(mGameObjectManager);
    MeshComponent::setMeshManager(mMeshManager);
    SpriteComponent::setSpriteManager(mSpriteManager);
    Sprite3D::setSpriteManager(mSpriteManager);
    Collider::setPhysics(mPhysics);
    PointLightComponent::setLightManager(mLightManager);
    TextBase::setDrawString(mTextDrawer);

    auto cam = GameObjectCreater::create("Camera");
    mCamera = cam->componentManager().getComponent<Camera>();

    mLightManager->createDirectionalLight();

    //初期シーンの設定
    createScene("Title");
}

void SceneManager::update() {
    //アップデートの最初で文字列削除
    DebugUtility::drawStringClear();
    mShouldDraw = true;

    //Escでゲーム終了
    if (Input::keyboard().getKeyDown(KeyCode::Escape)) {
        Game::quit();
    }

    //ポーズ中はデバッグだけアップデートを行う
    if (DebugUtility::pause()->isPausing()) {
        DebugUtility::update();
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
    //デバッグ
    DebugUtility::update();

    //シーン移行
    const auto& next = mCurrentScene->getNext();
    if (!next.empty()) {
        change(mCurrentScene->getObjectToNext());
        createScene(next);
        mShouldDraw = false;
    }
}

void SceneManager::draw() const {
    if (!mShouldDraw) {
        return;
    }
    //各テクスチャ上にレンダリング
    mRenderer->renderToTexture();
    //メッシュの一括描画
    mMeshManager->draw(*mCamera);
    //各テクスチャを参照してレンダリング
    mRenderer->renderFromTexture(*mCamera, *mLightManager);
    //ポイントライト描画準備
    mRenderer->renderPointLight();
    //ポイントライトの一括描画
    mLightManager->drawPointLights(*mCamera);
    //透明メッシュの描画
    mMeshManager->drawTransparent(*mCamera);

    //スプライト描画準備
    mRenderer->renderSprite();
    //3Dスプライト
    mRenderer->renderSprite3D();
    mSpriteManager->draw3Ds(mCamera->getView(), mCamera->getProjection());
    //2Dスプライト
    auto proj = Matrix4::identity;
    mRenderer->renderSprite2D(&proj);
    mSpriteManager->drawComponents(proj);
    //テキスト一括描画
    mTextDrawer->drawAll(proj);

#ifdef _DEBUG
    //レンダリング領域をデバッグに変更
    mRenderer->renderToDebug(&proj);
    //デバッグ表示
    DebugUtility::draw(proj);
#endif // _DEBUG
}

void SceneManager::change(const StringSet& tags) {
    mGameObjectManager->clearExceptSpecified(tags);
    mMeshManager->clear();
    mSpriteManager->clear();
}

void SceneManager::createScene(const std::string& name) {
    auto scene = GameObjectCreater::create(name);
    mCurrentScene = scene->componentManager().getComponent<Scene>();
}
