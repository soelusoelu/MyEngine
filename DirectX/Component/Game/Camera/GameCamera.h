#pragma once

#include "ILockOn.h"
#include "../../Component.h"
#include "../../../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>

class Camera;
class Time;

class GameCamera
    : public Component
{
public:
    GameCamera();
    ~GameCamera();
    virtual void start() override;
    virtual void lateUpdate() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    //カメラの注視点となるプレイヤーを設定する
    void setPlayer(const std::shared_ptr<GameObject>& player);
    //プレイヤーまでの距離を取得する
    float getDistanceToPlayer() const;

private:
    GameCamera(const GameCamera&) = delete;
    GameCamera& operator=(const GameCamera&) = delete;

    void calcRotation(const Vector3& lookAt, const Vector3& pos);
    void onUnlockOn();

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<GameObject> mPlayer;
    ILockOn* mLockOn;
    //ロックオン解除時の補間時間
    std::unique_ptr<Time> mUnlockOnLerpTimer;
    //カメラ回転速度
    float mRotateSpeed;
    //プレイヤーまでの距離
    float mToPlayerDistance;
    //注視点のY軸オフセット
    float mLookAtOffsetY;
    //開始時のカメラY軸位置
    float mStartPositionY;
};
