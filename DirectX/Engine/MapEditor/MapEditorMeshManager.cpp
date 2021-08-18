#include "MapEditorMeshManager.h"
#include "AssetsPlacement.h"
#include "../Camera/SimpleCamera.h"
#include "../DebugManager/DebugManager.h"
#include "../DebugManager/DebugLayer/DebugLayer.h"
#include "../DebugManager/DebugLayer/Hierarchy.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Mesh/MeshManager.h"

MapEditorMeshManager::MapEditorMeshManager()
    : mGameObjectManager(std::make_unique<GameObjectManager>(false))
    , mMeshManager(std::make_unique<MeshManager>())
    , mCamera(std::make_unique<SimpleCamera>())
    , mPlace(std::make_unique<AssetsPlacement>())
    , mEngineManagingClassGetter(nullptr)
{
    mCamera->setPosition(Vector3::up * 15.f + Vector3::back * 15.f);
    mCamera->lookAt(Vector3::forward * 5.f);
}

MapEditorMeshManager::~MapEditorMeshManager() = default;

void MapEditorMeshManager::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    if (mode == FileMode::LOAD) {
        mMeshManager->saveAndLoad(inObj, alloc, mode);
    }
}

void MapEditorMeshManager::initialize(
    IInspector* inspector,
    IEngineManagingClassGetter* managingGetter,
    IEngineFunctionChanger* modeChanger,
    const ICurrentSelectTextureGetter* textureGetter
) {
    mEngineManagingClassGetter = managingGetter;
    mMeshManager->initialize();
    mPlace->initialize(mGameObjectManager.get(), mMeshManager.get(), inspector, textureGetter);
    modeChanger->callbackChangeMode([&](EngineMode mode) { onModeChange(mode); });
}

void MapEditorMeshManager::update(
    EngineMode mode
) {
    if (mode == EngineMode::MAP_EDITOR) {
        mPlace->placeAsset(*mCamera, mMeshManager.get());
        mGameObjectManager->update();
    }
}

void MapEditorMeshManager::draw(
    EngineMode mode,
    const Vector3& dirLightDirection,
    const Vector3& dirLightColor
) const {
    if (mode == EngineMode::MAP_EDITOR) {
        mMeshManager->draw(
            mCamera->getView(),
            mCamera->getProjection(),
            mCamera->getPosition(),
            dirLightDirection,
            dirLightColor
        );
    }
}

const IGameObjectsGetter* MapEditorMeshManager::getGameObjects() const {
    return mGameObjectManager.get();
}

void MapEditorMeshManager::onModeChange(EngineMode mode) {
    if (mode == EngineMode::MAP_EDITOR) {
        mMeshManager->registerThisToMeshRenderer();
        //ヒエラルキーにマップエディタで使用するゲームオブジェクト管理者を設定する
        mEngineManagingClassGetter->debug().getDebugLayer().hierarchy().setGameObjectsGetter(mGameObjectManager.get());
    }
}
