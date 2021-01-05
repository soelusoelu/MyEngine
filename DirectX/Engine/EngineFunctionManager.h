#pragma once

#include "DebugMode.h"
#include "Pause/IPause.h"
#include "../GameObject/IGameObjectsGetter.h"
#include "../Math/Math.h"
#include "../Mesh/IMeshesGetter.h"
#include "../System/FpsCounter/IFpsGetter.h"
#include <memory>
#include <string>
#include <rapidjson/document.h>

class Camera;
class Renderer;
class DebugManager;
class Pause;
class AssetsRenderTextureManager;
class SceneMeshOperator;

//エンジン機能統括クラス
class EngineFunctionManager {
public:
    EngineFunctionManager();
    ~EngineFunctionManager();
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj);
    //初期化
    void initialize(const std::shared_ptr<Camera>& camera, const IGameObjectsGetter* gameObjctsGetter, const IMeshesGetter* meshesGetter, const IFpsGetter* fpsGetter);
    //アップデート前処理
    void preUpdateProcess();
    //毎フレーム更新
    void update();
    //シーン遷移時
    void changeScene(const std::string& scene);
    //2D関連の描画
    void draw(const Renderer& renderer, Matrix4& proj) const;
    //3D関連の描画
    void draw3D(const Renderer& renderer, const Matrix4& viewProj) const;
    //デバッグ機能へのアクセス
    DebugManager& debug() const;
    //ポーズ機能へのアクセス
    IPause& pause() const;
    //アセットテクスチャ管理者を取得する
    AssetsRenderTextureManager& getAssetsRenderTextureManager() const;

private:
    EngineFunctionManager(const EngineFunctionManager&) = delete;
    EngineFunctionManager& operator=(const EngineFunctionManager&) = delete;

private:
    std::unique_ptr<DebugManager> mDebugManager;
    std::unique_ptr<Pause> mPause;
    std::unique_ptr<AssetsRenderTextureManager> mAssetsRenderTextureManager;
    std::unique_ptr<SceneMeshOperator> mSceneMeshOperator;
    DebugMode mMode;
};
