#pragma once

#include "IAssetsRenderTexturesGetter.h"
#include "../../Math/Math.h"
#include <memory>

class AssetsTexturesSelector;
class MeshRenderOnTexture;

//ゲーム画面にアセットを配置するクラス
class AssetsPlacement {
public:
    AssetsPlacement();
    ~AssetsPlacement();
    void initialize(const IAssetsRenderTexturesGetter* getter);
    void update();

private:
    AssetsPlacement(const AssetsPlacement&) = delete;
    AssetsPlacement& operator=(const AssetsPlacement&) = delete;

    //アセットを配置する
    void placeAsset();
    //アセットを配置する条件が整っているか
    bool placeConditions() const;

private:
    std::unique_ptr<AssetsTexturesSelector> mSelector;
    std::shared_ptr<MeshRenderOnTexture> mCurrentSelectTexture;
};
