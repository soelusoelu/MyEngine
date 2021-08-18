#pragma once

#include "IMoveDirectionGetter.h"
#include "IPlayerMove.h"
#include "../../Component.h"
#include "../../../Device/Function.h"
#include "../../../Math/Math.h"
#include <functional>
#include <memory>

class Camera;
class SkinMeshComponent;
class PlayerWalk;
class PlayerDash;
class BulletShooter;

class PlayerMove
    : public Component
    , public IPlayerMove
    , public IMoveDirectionGetter
{
public:
    PlayerMove();
    ~PlayerMove();
    //Component
    virtual void start() override;

    //IMoveDirectionGetter
    virtual const Vector3& getMoveDirectionInputedLast() const override;

    void originalUpdate();
    //動いているか
    bool isMoving() const;
    //歩いているか
    bool isWalking() const;
    //走っているか
    bool isDashing() const;
    //移動停止した際のコールバック
    void callbackToStop(const std::function<void()>& callback);

private:
    PlayerMove(const PlayerMove&) = delete;
    PlayerMove& operator=(const PlayerMove&) = delete;

    //IPlayerMove
    virtual void move(float moveSpeed) override;
    virtual void rotateToMoveDirection() override;

    //移動方向を求める
    void calcMoveDirection(const Vector2& leftStickValue);
    //移動停止処理
    void stop();
    //動ける状態か
    bool canMove(const Vector2& leftStickValue) const;

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::shared_ptr<PlayerWalk> mWalk;
    std::shared_ptr<PlayerDash> mDash;
    std::shared_ptr<BulletShooter> mShooter;
    Function<void()> mCallbackToStop;
    //最後に入力された移動方向
    Vector3 mMoveDirectionInputedLast;
};
