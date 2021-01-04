#pragma once

#include "ICurrentSelectTextureGetter.h"
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
class MeshRenderOnTexture;

//メッシュ描画テクスチャ関連総管理クラス
class AssetsRenderTextureManager : public ICurrentSelectTextureGetter {
public:
    AssetsRenderTextureManager();
    ~AssetsRenderTextureManager();
    virtual const MeshRenderOnTexture& getCurrentSelectTexture() const override;
    virtual bool selectedTexture() const override;
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
    //初期化
    void initialize();
    //一通りの初期化後の初期化処理
    void afterInitialize(const std::shared_ptr<Camera>& camera, const IMeshesGetter* getter);
    //毎フレーム更新
    void update();
    //メッシュを描画する
    void drawMeshes() const;
    //テクスチャを描画する
    void drawTextures(const Matrix4& proj) const;

private:
    AssetsRenderTextureManager(const AssetsRenderTextureManager&) = delete;
    AssetsRenderTextureManager& operator=(const AssetsRenderTextureManager&) = delete;

private:
    std::unique_ptr<AssetsRenderTextureList> mTextureList;
    std::unique_ptr<AssetsRenderTextureAdder> mTextureAdder;
    std::unique_ptr<AssetsRenderTextureDeleter> mDeleter;
    std::unique_ptr<AssetsTexturesSelector> mSelector;
    std::unique_ptr<AssetsPlacement> mPlacement;
    std::shared_ptr<MeshRenderOnTexture> mCurrentSelectTexture;
};
