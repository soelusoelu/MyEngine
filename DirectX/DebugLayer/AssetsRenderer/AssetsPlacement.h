#pragma once

#include "IAssetsRenderTexturesGetter.h"
#include "../../Math/Math.h"
#include "../../Mesh/IMeshesGetter.h"
#include <memory>

class Camera;
class GameObject;
class AssetsTexturesSelector;
class MeshRenderOnTexture;

//ゲーム画面にアセットを配置するクラス
class AssetsPlacement {
public:
    AssetsPlacement();
    ~AssetsPlacement();
    void initialize(const IAssetsRenderTexturesGetter* getter);
    void afterInitialize(const std::shared_ptr<Camera>& camera, const IMeshesGetter* getter);
    void update();

private:
    AssetsPlacement(const AssetsPlacement&) = delete;
    AssetsPlacement& operator=(const AssetsPlacement&) = delete;

    //アセットを配置する
    void placeAsset();
    //アセットを配置する場所を決める
    void decideAssetPlacePosition(GameObject& asset) const;
    //アセットを配置する条件が整っているか
    bool placeConditions() const;

private:
    std::shared_ptr<Camera> mCamera;
    const IMeshesGetter* mMeshesGetter;
    std::unique_ptr<AssetsTexturesSelector> mSelector;
    std::shared_ptr<MeshRenderOnTexture> mCurrentSelectTexture;
};
