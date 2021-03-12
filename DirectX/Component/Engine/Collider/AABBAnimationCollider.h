#pragma once

#include "Collider.h"
#include "../../../Collision/Collision.h"
#include "../../../Math/Math.h"
#include "../../../Mesh/IMesh.h"
#include <array>
#include <memory>

class SkinMeshComponent;

class AABBAnimationCollider
    : public Collider
    , public std::enable_shared_from_this<AABBAnimationCollider> {

public:
    AABBAnimationCollider(GameObject& gameObject);
    ~AABBAnimationCollider();
    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void finalize() override;
    virtual void onEnable(bool value) override;
    virtual void drawInspector() override;

    //AABBを取得する
    const AABB& getAABB() const;
    //当たり判定を可視化するか
    void setRenderCollision(bool value);

private:
    AABBAnimationCollider(const AABBAnimationCollider&) = delete;
    AABBAnimationCollider& operator=(const AABBAnimationCollider&) = delete;

    //AABBを更新する
    void updateAABB();
    //AABBの点を更新する
    void updatePoints();
    //当たり判定を可視化する
    void renderCollision();

private:
    //当たり判定であるAABB
    AABB mAABB;
    //アニメーションコンポーネント
    std::shared_ptr<SkinMeshComponent> mAnimation;
    //AABBの各点
    std::array<Vector3, BoxConstantGroup::POINTS_NUM> mPoints;
    //当たり判定を表示するか
    bool mIsRenderCollision;
};
