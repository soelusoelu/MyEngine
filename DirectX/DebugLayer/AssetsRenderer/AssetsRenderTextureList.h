#pragma once

#include "IAddAssets.h"
#include "IAssetsRenderTexturesGetter.h"
#include "../../Math/Math.h"
#include <rapidjson/document.h>
#include <list>
#include <memory>
#include <string>
#include <unordered_set>

//メッシュを描画したテクスチャ管理クラス
class AssetsRenderTextureList : public IAddAssets, public IAssetsRenderTexturesGetter {
public:
    AssetsRenderTextureList();
    ~AssetsRenderTextureList();
    virtual void add(const std::string& fileName, const std::string& directoryPath) override;
    virtual const MeshRenderOnTexturePtrList& getTextures() const override;
    void initialize();
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
    void update();
    //メッシュをテクスチャに描画する
    void drawMeshOnTexture() const;
    //メッシュ描画済みテクスチャを描画する
    void drawTexture(const Matrix4& proj) const;

private:
    AssetsRenderTextureList(const AssetsRenderTextureList&) = delete;
    AssetsRenderTextureList& operator=(const AssetsRenderTextureList&) = delete;

    //テクスチャを作成し格納する
    void createTexture(const std::string& fileName, const std::string& directoryPath);
    void createTexture(const std::string& filePath);
    //ファイルパスが読み込み済みか
    bool loadedFilePath(const std::string& filePath) const;

private:
    MeshRenderOnTexturePtrList mTextures;
    MeshRenderOnTexturePtrList mNonDrawTextures;
    std::unordered_set<std::string> mTexturesFilePath;
    Matrix4 mViewProj;

    //縦横共通サイズ
    int mTextureSize;
    //テクスチャの最大列表示数
    int mColumnDisplayLimit;
    //テクスチャの標示間隔
    int mTextureDisplayInterval;
};
