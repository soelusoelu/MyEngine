#pragma once

#include "IAddAssets.h"
#include "IAssetsRenderTexturesGetter.h"
#include "../../Math/Math.h"
#include <rapidjson/document.h>
#include <list>
#include <memory>
#include <string>

//メッシュを描画したテクスチャ管理クラス
class AssetsRenderTextureList : public IAddAssets, public IAssetsRenderTexturesGetter {
public:
    AssetsRenderTextureList();
    ~AssetsRenderTextureList();
    virtual void add(const std::string& fileName, const std::string& directoryPath) override;
    virtual const MeshRenderOnTexturePtrList& getTextures() const override;
    void loadProperties(const rapidjson::Value& inObj);
    void update();
    //メッシュをテクスチャに描画する
    void drawMeshOnTexture() const;
    //メッシュ描画済みテクスチャを描画する
    void drawTexture(const Matrix4& proj) const;

private:
    AssetsRenderTextureList(const AssetsRenderTextureList&) = delete;
    AssetsRenderTextureList& operator=(const AssetsRenderTextureList&) = delete;

private:
    MeshRenderOnTexturePtrList mTextures;
    MeshRenderOnTexturePtrList mNonDrawTextures;
    Matrix4 mViewProj;

    //縦横共通サイズ
    int mTextureSize;
    //テクスチャの最大列表示数
    int mColumnDisplayLimit;
    //テクスチャの標示間隔
    int mTextureDisplayInterval;
};
