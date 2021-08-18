#pragma once

#include "IPlayerMove.h"
#include "../../Component.h"
#include "../../../Device/Function.h"
#include "../../../Math/Math.h"
#include <functional>
#include <memory>

class SkinMeshComponent;
class BulletShooter;

class PlayerWalk
    : public Component
{
public:
    PlayerWalk();
    ~PlayerWalk();
    virtual void start() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    //歩行処理
    void walk(IPlayerMove& playerMove);
    //歩いているか
    bool isWalking() const;
    //カメラ角度を設定する
    void setCameraRotation(const Quaternion& cameraRotation);
    //歩行に移行した際のコールバック
    void callbackToWalk(const std::function<void()>& callback);

private:
    PlayerWalk(const PlayerWalk&) = delete;
    PlayerWalk& operator=(const PlayerWalk&) = delete;

    //歩行中の回転処理
    void rotate(IPlayerMove& playerMove);
    //モーション変更時のコールバック
    void onChangeMotion();
    //ADS時に呼ばれる
    void onAds();

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::shared_ptr<BulletShooter> mBulletShooter;
    const Quaternion* mCameraRotation;
    Function<void()> mCallbackToWalk;
    float mWalkSpeed;
    bool mIsWalking;
};
