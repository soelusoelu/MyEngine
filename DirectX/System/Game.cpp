#include "Game.h"
#include "SceneManager.h"
#include "Texture.h"
#include "Window.h"
#include "World.h"
#include "../DebugLayer/DebugUtility.h"
#include "../Device/FPSCounter.h"
#include "../Device/Random.h"
#include "../DirectX/DirectX.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Input/InputUtility.h"
#include "../Utility/Directory.h"
#include "../Utility/FileUtil.h"
#include "../Utility/LevelLoader.h"

Game::Game() :
    mWindow(nullptr),
    mSceneManager(nullptr),
    mFPSCounter(nullptr),
    mInstance(nullptr),
    mhWnd(nullptr) {
}

Game::~Game() {
    Texture::finalize();
    GameObjectCreater::finalize();
    InputUtility::finalize();
    DebugUtility::finalize();
    World::instance().finalize();
    DirectX::instance().finalize();
}

void Game::run(HINSTANCE hInstance) {
    World::instance().directory().initialize();

    mInstance = hInstance;
    initialize();

    MSG msg = { 0 };
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            mainLoop();
        }
    }
}

void Game::loadProperties(const rapidjson::Value& inObj) {
    mWindow->loadProperties(inObj);
    mFPSCounter->loadProperties(inObj);
    DebugUtility::loadProperties(inObj);
    InputUtility::loadProperties(inObj);
    mSceneManager->loadProperties(inObj);
}

void Game::quit() {
    PostQuitMessage(0);
}

void Game::initialize() {
    mWindow = std::make_unique<Window>();

    mFPSCounter = std::make_unique<FPSCounter>();
    DebugUtility::create();
    InputUtility::create();
    mSceneManager = std::make_unique<SceneManager>();

    //ファイルから値を読み込む
    LevelLoader::loadGlobal(this, "Global.json");

    mWindow->createWindow(mInstance);
    mhWnd = mWindow->gethWnd();

    DirectX::instance().initialize(mhWnd);

    Random::initialize();
    DebugUtility::initialize();
    InputUtility::initialize(mhWnd);
    GameObjectCreater::initialize();
    mSceneManager->initialize();
}

void Game::mainLoop() {
    auto& dx = DirectX::instance();
    dx.clearRenderTarget();
    dx.clearDepthStencilView();

    InputUtility::update();
    mWindow->update();

    mSceneManager->update();
    mSceneManager->draw();

    mFPSCounter->fixedFrame();
    dx.present();
}
