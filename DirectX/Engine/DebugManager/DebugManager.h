#pragma once

#include "../EngineMode.h"
#include "../Pause/IPause.h"
#include "../../Device/FileOperator.h"
#include "../../GameObject/IGameObjectsGetter.h"
#include "../../Math/Math.h"
#include "../../System/FpsCounter/IFpsGetter.h"
#include <memory>

class Camera;
class DirectionalLight;
class Renderer;
class DrawString;
class DebugLayer;

class DebugManager
    : public FileOperator
{
public:
    DebugManager();
    ~DebugManager();

    //初期化
    void initialize(
        const IGameObjectsGetter* gameObjectsGetter,
        const IFpsGetter* fpsGetter,
        const IPause* pause
    );

    //アップデート前処理
    void preUpdateProcess();
    //毎フレーム更新
    void update();

    //2D関連の描画
    void draw2D(const Renderer& renderer, Matrix4& proj) const;
    //2Dデバッグ関連の描画
    void drawDebug2D(EngineMode mode, Matrix4& proj) const;

    //3D関連の描画
    void draw3D(
        EngineMode mode,
        const Renderer& renderer,
        const Matrix4& viewProj
    ) const;

    //デバッグレイヤーを取得する
    DebugLayer& getDebugLayer() const;

private:
    DebugManager(const DebugManager&) = delete;
    DebugManager& operator=(const DebugManager&) = delete;

    virtual void childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

private:
    std::unique_ptr<DrawString> mStringDrawer;
    std::unique_ptr<DebugLayer> mDebugLayer;
};
