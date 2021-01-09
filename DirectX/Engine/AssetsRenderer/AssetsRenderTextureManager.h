#pragma once

#include "ICallbackSelectAssetsTexture.h"
#include "ICurrentSelectTextureGetter.h"
#include "../EngineMode.h"
#include "../DebugManager/DebugLayer/Inspector/IInspectorTargetSetter.h"
#include "../../Math/Math.h"
#include "../../Mesh/IMeshesGetter.h"
#include <rapidjson/document.h>
#include <memory>

class Camera;
class AssetsRenderTextureList;
class AssetsRenderTextureAdder;
class AssetsRenderTextureDeleter;
class AssetsTexturesSelector;
class AssetsPlacement;
class AssetsRenderTexture;

//メッシュ描画テクスチャ関連総管理クラス
class AssetsRenderTextureManager : public ICurrentSelectTextureGetter {
public:
    AssetsRenderTextureManager();
    ~AssetsRenderTextureManager();
    virtual const AssetsRenderTexture& getCurrentSelectTexture() const override;
    virtual bool selectedTexture() const override;
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
    //初期化
    void initialize(const std::shared_ptr<Camera>& camera, IInspectorTargetSetter* inspector, const IMeshesGetter* getter);
    //毎フレーム更新
    void update(EngineMode mode);
    //メッシュを描画する
    void drawMeshes(EngineMode mode) const;
    //テクスチャを描画する
    void drawTextures(EngineMode mode, const Matrix4& proj) const;
    //アセットテクスチャ選択コールバックを取得する
    ICallbackSelectAssetsTexture* getCallbackSelectAssetsTexture() const;

private:
    AssetsRenderTextureManager(const AssetsRenderTextureManager&) = delete;
    AssetsRenderTextureManager& operator=(const AssetsRenderTextureManager&) = delete;

private:
    std::unique_ptr<AssetsRenderTextureList> mTextureList;
    std::unique_ptr<AssetsRenderTextureAdder> mTextureAdder;
    std::unique_ptr<AssetsRenderTextureDeleter> mDeleter;
    std::unique_ptr<AssetsTexturesSelector> mSelector;
    std::unique_ptr<AssetsPlacement> mPlacement;
    std::shared_ptr<AssetsRenderTexture> mCurrentSelectTexture;
};
