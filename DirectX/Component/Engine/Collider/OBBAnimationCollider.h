#pragma once

#include "Collider.h"
#include "../Mesh/IAnimationCPU.h"
#include "../../../Collision/Collision.h"
#include "../../../Math/Math.h"
#include <memory>
#include <vector>

class MeshComponent;
class AnimationCPU;

//OBBが持つ情報群
struct OBBInfomation {
    //OBB
    OBB obb;
    //OBBの結合対象
    std::vector<unsigned> concatenateTargets;
    //OBBの各点
    BoxPoints points;
    //アクティブか
    bool isActive = true;
};

class OBBAnimationCollider :
    public Collider
{
public:
    OBBAnimationCollider(GameObject& gameObject);
    ~OBBAnimationCollider();
    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void finalize() override;
    virtual void onEnable(bool value) override;
    virtual void drawInspector() override;

    //指定したOBBを結合する
    void concatenate(unsigned a, unsigned b);
    //OBBを取得する
    const OBB& getOBB(unsigned index) const;
    //当たり判定を可視化するか
    void setRenderCollision(bool value);

private:
    OBBAnimationCollider(const OBBAnimationCollider&) = delete;
    OBBAnimationCollider& operator=(const OBBAnimationCollider&) = delete;

    //OBBを更新する
    void computeOBB();
    //OBBのサイズを更新する
    void updateOBB(unsigned target, unsigned index);
    //OBBのサイズを作成し直す
    void resizeOBB(unsigned meshIndex);
    //OBBを作成する
    void createOBB();
    //行列を収集する
    Matrix3 collectMatrix(unsigned meshIndex);
    //固有ベクトルを取得する
    Matrix3 getEigenvector(Matrix3& mat);
    //行列の各成分の絶対値の最大値とその位置を求める
    void getMaxValueForMatrix(const Matrix3& mat, float& outMax, unsigned& outRow, unsigned& outColumn);
    //OBBの点を更新する
    void updatePoints();
    //当たり判定を可視化する
    void renderCollision();

private:
    //当たり判定であるOBB情報群
    std::vector<OBBInfomation> mOBBs;
    //メッシュコンポーネント
    std::shared_ptr<MeshComponent> mMesh;
    //アニメーションコンポーネント
    std::shared_ptr<AnimationCPU> mAnimationCPU;
    //当たり判定を表示するか
    bool mIsRenderCollision;
};
