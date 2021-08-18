#pragma once

#include "IEngineFunctionChanger.h"
#include "Pause/IPause.h"

class DebugManager;
class AssetsRenderTextureManager;
class MapEditorMeshManager;

//エンジン機能統括クラスが管理しているクラスにアクセスする
class IEngineManagingClassGetter {
public:
    virtual ~IEngineManagingClassGetter() = default;
    //エンジンモード変更者へのアクセス
    virtual IEngineFunctionChanger& getModeChanger() const = 0;
    //デバッグ機能へのアクセス
    virtual DebugManager& debug() const = 0;
    //ポーズ機能へのアクセス
    virtual IPause& pause() const = 0;
    //アセットテクスチャ管理者を取得する
    virtual AssetsRenderTextureManager& getAssetsRenderTextureManager() const = 0;
    //マップエディタ管理者を取得する
    virtual MapEditorMeshManager& getMapEditorMeshManager() const = 0;
};