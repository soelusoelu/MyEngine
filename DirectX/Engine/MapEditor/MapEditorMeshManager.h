#pragma once

#include "../EngineMode.h"
#include "../IEngineFunctionChanger.h"
#include "../IEngineManagingClassGetter.h"
#include "../AssetsRenderer/ICurrentSelectTextureGetter.h"
#include "../DebugManager/DebugLayer/Inspector/IInspector.h"
#include "../../GameObject/IGameObjectsGetter.h"
#include "../../Math/Math.h"
#include "../../Utility/FileMode.h"
#include <rapidjson/document.h>
#include <memory>

class GameObjectManager;
class MeshManager;
class SimpleCamera;
class AssetsPlacement;

class MapEditorMeshManager {
public:
    MapEditorMeshManager();
    ~MapEditorMeshManager();
    void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode);

    void initialize(
        IInspector* inspector,
        IEngineManagingClassGetter* managingGetter,
        IEngineFunctionChanger* modeChanger,
        const ICurrentSelectTextureGetter* textureGetter
    );

    void update(
        EngineMode mode
    );

    void draw(
        EngineMode mode,
        const Vector3& dirLightDirection,
        const Vector3& dirLightColor
    ) const;

    //IGameObjectsGetterを取得する
    const IGameObjectsGetter* getGameObjects() const;

private:
    MapEditorMeshManager(const MapEditorMeshManager&) = delete;
    MapEditorMeshManager& operator=(const MapEditorMeshManager&) = delete;

    //エンジンモード変更時
    void onModeChange(EngineMode mode);

private:
    std::unique_ptr<GameObjectManager> mGameObjectManager;
    std::unique_ptr<MeshManager> mMeshManager;
    std::unique_ptr<SimpleCamera> mCamera;
    std::unique_ptr<AssetsPlacement> mPlace;
    IEngineManagingClassGetter* mEngineManagingClassGetter;
};
