#include "MapEditorMeshManager.h"
#include "AssetsPlacement.h"
#include "../Camera/SimpleCamera.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Mesh/MeshManager.h"

MapEditorMeshManager::MapEditorMeshManager()
    : mGameObjectManager(std::make_unique<GameObjectManager>(false))
    , mMeshManager(std::make_unique<MeshManager>(false))
    , mCamera(std::make_unique<SimpleCamera>())
    , mPlace(std::make_unique<AssetsPlacement>())
{
}

MapEditorMeshManager::~MapEditorMeshManager() = default;

void MapEditorMeshManager::initialize(
    IInspectorTargetSetter* inspector,
    const ICurrentSelectTextureGetter* textureGetter
) {
    mMeshManager->createShadowMap();
    mPlace->initialize(mGameObjectManager.get(), mMeshManager.get(), inspector, textureGetter);
}

void MapEditorMeshManager::update(
    EngineMode mode
) {
    if (mode == EngineMode::MAP_EDITOR) {
        mPlace->placeAsset(*mCamera, mMeshManager.get());
    }

    mGameObjectManager->update();
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
