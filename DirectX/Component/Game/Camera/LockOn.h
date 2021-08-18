#pragma once

#include "ILockOn.h"
#include "../../Component.h"
#include "../../../Device/Function.h"
#include "../../../Math/Math.h"
#include <memory>
#include <vector>

class GameObject;
class Camera;
class GameCamera;
class Time;

class LockOn
    : public Component
    , public ILockOn
{
    using Player = std::shared_ptr<GameObject>;
    using Enemy = std::shared_ptr<GameObject>;
    using Enemys = std::vector<Enemy>;

public:
    LockOn();
    ~LockOn();
    //Component
    virtual void start() override;
    virtual void update() override;
    virtual void lateUpdate() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    //ILockOn
    virtual bool isLockOn() const override;
    virtual const Transform3D& getLockOnTargetTransform() const override;
    virtual void callbackLockOn(const std::function<void()>& callback) override;
    virtual void callbackUnlockOn(const std::function<void()>& callback) override;

    void setPlayer(const Player& player);
    void setEnemys(const Enemys& enemys);

private:
    LockOn(const LockOn&) = delete;
    LockOn& operator=(const LockOn&) = delete;

    //ロックオン処理
    void lockOn();
    //ロックオン時のカメラ更新
    void lockOnUpdate();
    //ロックオン範囲内か
    bool isLockOnRange(const Vector3& enemyPosition) const;

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<GameCamera> mGameCamera;
    Player mPlayer;
    Enemys mEnemys;
    Enemy mLockOnTarget;
    Function<void()> mCallbackLockOn;
    Function<void()> mCallbackUnlockOn;
    //ロックオン範囲角度
    float mLockOnAngle;
    //ロックオン中か
    bool mIsLockOn;
    //ロックオン時の注視点Y軸オフセット
    float mLookAtOffsetY;
    //ロックオン時のカメラ位置Y軸オフセット
    float mCameraOffsetY;
    //ロックオン時の補間時間
    std::unique_ptr<Time> mLerpTimer;
};
