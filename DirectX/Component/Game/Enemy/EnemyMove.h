#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include <memory>

class GameObject;
class SkinMeshComponent;

class EnemyMove
    : public Component
{
public:
    EnemyMove();
    ~EnemyMove();
    EnemyMove(const EnemyMove&) = delete;
    EnemyMove& operator=(const EnemyMove&) = delete;

    virtual void start() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;

    void move();
    void rotate();
    bool shouldRotate() const;

private:
    //現在のモーションがTPose(歩行)か
    bool isCurrentMotionTPose() const;
    //自身からプレイヤーへの正規化ベクトルを取得する
    Vector3 getToPlayer() const;

private:
    std::shared_ptr<GameObject> mPlayer;
    std::shared_ptr<SkinMeshComponent> mAnimation;
    float mMoveSpeed;
    float mRotateSpeed;
    float mAngleNeedsRotate;
};
