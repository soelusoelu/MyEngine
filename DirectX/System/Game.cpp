#include "Game.h"
#include "SceneManager.h"
#include "Texture.h"
#include "Window.h"
#include "../DebugLayer/DebugUtility.h"
#include "../Device/DrawString.h"
#include "../Device/FPSCounter.h"
#include "../Device/Random.h"
#include "../Device/Renderer.h"
#include "../Device/Time.h"
#include "../DirectX/DirectX.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Input/InputUtility.h"
#include "../Utility/Directory.h"
#include "../Utility/FileUtil.h"
#include "../Utility/LevelLoader.h"

Game::Game() :
    mWindow(nullptr),
    mRenderer(nullptr),
    mSceneManager(nullptr),
    mFPSCounter(nullptr),
    mInstance(nullptr),
    mhWnd(nullptr) {
}

Game::~Game() {
    InputUtility::finalize();
    Texture::finalize();
    DebugUtility::finalize();
}

void Game::run(HINSTANCE hInstance) {
    Singleton<Directory>::instance().initialize();

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
    mRenderer->loadProperties(inObj);
    mFPSCounter->loadProperties(inObj);
    DebugUtility::loadProperties(inObj);
    InputUtility::loadProperties(inObj);
}

void Game::quit() {
    PostQuitMessage(0);
}

void Game::initialize() {
    mWindow = std::make_unique<Window>();

    mRenderer = std::make_shared<Renderer>();
    mFPSCounter = std::make_unique<FPSCounter>();
    Singleton<GameObjectFactory>::instance().initialize(mRenderer);
    DebugUtility::create();
    InputUtility::create();
    mSceneManager = std::make_unique<SceneManager>(mRenderer);

    //ファイルから値を読み込む
    Singleton<LevelLoader>::instance().loadGlobal(this, "Global.json");

    mWindow->createWindow(mInstance);
    mhWnd = mWindow->gethWnd();

    Singleton<DirectX>::instance().initialize(mhWnd);
    mRenderer->initialize();

    Random::initialize();
    DebugUtility::initialize();
    InputUtility::initialize(mhWnd);
    mSceneManager->initialize();
}

void Game::mainLoop() {
    Singleton<DirectX>::instance().clearRenderTarget();
    Singleton<DirectX>::instance().clearDepthStencilView();

    InputUtility::update();
    mWindow->update();

    mSceneManager->update();
    mSceneManager->draw();

    mFPSCounter->fixedFrame();
    Singleton<DirectX>::instance().present();
}
