#pragma once

#include "IAssetsRenderTextureList.h"
#include "../../Math/Math.h"
#include <rapidjson/document.h>
#include <list>
#include <memory>
#include <string>

class MeshRenderOnTexture;

//メッシュを描画したテクスチャ管理クラス
class AssetsRenderTextureList : public IAssetsRenderTextureList {
    using MeshRenderOnTexturePtr = std::shared_ptr<MeshRenderOnTexture>;
    using MeshRenderOnTexturePtrList = std::list<MeshRenderOnTexturePtr>;

public:
    AssetsRenderTextureList();
    ~AssetsRenderTextureList();
    virtual void add(const std::string& fileName, const std::string& directoryPath) override;
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
