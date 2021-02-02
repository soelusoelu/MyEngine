#pragma once

#include "../AssetsRenderer/ICurrentSelectTextureGetter.h"
#include "../DebugManager/DebugLayer/Inspector/IInspectorTargetSetter.h"
#include "../../GameObject/IGameObjectAdder.h"
#include "../../Math/Math.h"
#include "../../Mesh/IMeshAdder.h"
#include "../../Mesh/IMeshesGetter.h"
#include <memory>

class SimpleCamera;
class GameObject;
class Transform3D;

//ゲーム画面にアセットを配置するクラス
class AssetsPlacement {
public:
    AssetsPlacement();
    ~AssetsPlacement();

    void initialize(
        IGameObjectAdder* gameObjectAdder,
        IMeshAdder* meshAdder,
        IInspectorTargetSetter* inspector,
        const ICurrentSelectTextureGetter* textureGetter
    );

    //アセットを配置する
    void placeAsset(
        const SimpleCamera& camera,
        const IMeshesGetter* meshesGetter
    );

private:
    AssetsPlacement(const AssetsPlacement&) = delete;
    AssetsPlacement& operator=(const AssetsPlacement&) = delete;

    //アセットを配置する場所を決める
    void decideAssetPlaceTransform(
        const SimpleCamera& camera,
        const IMeshesGetter* meshesGetter,
        const std::shared_ptr<GameObject>& asset
    ) const;

    //アセットを配置する位置を決める
    void decideAssetPlacePosition(
        const GameObject& target,
        const Transform3D& hitObject,
        const Vector3& hitPoint
    ) const;

    //配置したアセットの向きを決める
    void decideAssetPlaceRotation(
        const GameObject& target,
        const Vector3& hitPolygonNormal
    ) const;

    //アセットを配置する条件が整っているか
    bool placeConditions() const;

private:
    IGameObjectAdder* mGameObjectAdder;
    IMeshAdder* mMeshAdder;
    IInspectorTargetSetter* mInspector;
    const ICurrentSelectTextureGetter* mTextureGetter;
};
