#pragma once

#include "../../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>

class AssetsRenderTextureList;
class AssetsRenderTextureAdder;

//メッシュ描画テクスチャ関連総管理クラス
class AssetsRenderTextureManager {
public:
    AssetsRenderTextureManager();
    ~AssetsRenderTextureManager();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    void update();
    void drawMeshes();
    void drawTextures(const Matrix4& proj);

private:
    AssetsRenderTextureManager(const AssetsRenderTextureManager&) = delete;
    AssetsRenderTextureManager& operator=(const AssetsRenderTextureManager&) = delete;

private:
    std::unique_ptr<AssetsRenderTextureList> mTextureList;
    std::unique_ptr<AssetsRenderTextureAdder> mTextureAdder;
};
