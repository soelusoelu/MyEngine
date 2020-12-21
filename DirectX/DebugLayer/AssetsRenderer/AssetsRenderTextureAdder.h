#pragma once

#include "IAssetsRenderTextureList.h"
#include "../../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>

class SpriteButton;

//メッシュを描画したテクスチャ追加クラス
class AssetsRenderTextureAdder {
public:
    AssetsRenderTextureAdder();
    ~AssetsRenderTextureAdder();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize(IAssetsRenderTextureList* textureList);
    void update();
    void draw(const Matrix4& proj);

private:
    AssetsRenderTextureAdder(const AssetsRenderTextureAdder&) = delete;
    AssetsRenderTextureAdder& operator=(const AssetsRenderTextureAdder&) = delete;

    void onClickButton();

private:
    std::unique_ptr<SpriteButton> mButton;
    IAssetsRenderTextureList* mTextureList;
    std::string mSpriteFilePath;
    Vector2 mRenderPosition;
};
