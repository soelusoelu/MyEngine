#pragma once

#include "Collider.h"
#include "../Mesh/IAnimationCPU.h"
#include "../../../Collision/Collision.h"
#include "../../../Math/Math.h"
#include <array>
#include <memory>

class MeshComponent;
class AnimationCPU;

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
    //メッシュから最小、最大点を割り出す
    void computeMinMax(Vector3& outMin, Vector3& outMax, const MeshVertexPositions& positions);
    //AABBの点を更新する
    void updatePoints();
    //当たり判定を可視化する
    void renderCollision();

private:
    //当たり判定であるAABB
    AABB mAABB;
    //メッシュコンポーネント
    std::shared_ptr<MeshComponent> mMesh;
    //アニメーションコンポーネント
    std::shared_ptr<AnimationCPU> mAnimationCPU;
    //AABBの各点
    std::array<Vector3, BoxConstantGroup::POINTS_NUM> mPoints;
    //当たり判定を表示するか
    bool mIsRenderCollision;
};
