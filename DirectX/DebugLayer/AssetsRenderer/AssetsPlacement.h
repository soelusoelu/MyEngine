#pragma once

#include "ICurrentSelectTextureGetter.h"
#include "../../Math/Math.h"
#include "../../Mesh/IMeshesGetter.h"
#include <memory>

class Camera;
class GameObject;

//ゲーム画面にアセットを配置するクラス
class AssetsPlacement {
public:
    AssetsPlacement();
    ~AssetsPlacement();
    void initialize(const ICurrentSelectTextureGetter* getter);
    void afterInitialize(const std::shared_ptr<Camera>& camera, const IMeshesGetter* getter);
    void update();

private:
    AssetsPlacement(const AssetsPlacement&) = delete;
    AssetsPlacement& operator=(const AssetsPlacement&) = delete;

    //アセットを配置する
    void placeAsset();
    //アセットを配置する場所を決める
    void decideAssetPlacePosition(const std::shared_ptr<GameObject>& asset) const;
    //アセットを配置する条件が整っているか
    bool placeConditions() const;

private:
    const ICurrentSelectTextureGetter* mTextureGetter;
    std::shared_ptr<Camera> mCamera;
    const IMeshesGetter* mMeshesGetter;
};
