#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include <memory>

class Camera;
class GameObject;

class TPSCamera
    : public Component
{
public:
    TPSCamera();
    ~TPSCamera();
    TPSCamera(const TPSCamera&) = delete;
    TPSCamera& operator=(const TPSCamera&) = delete;

    virtual void start() override;
    virtual void update() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;

    void setPlayer(const std::shared_ptr<GameObject>& player);

private:
    void calcLookAt() const;
    void calcRotation(const Vector2& rightStick) const;
    void calcPosition() const;
    Vector3 getPlayerHeadPoint() const;
    Vector3 getCalcPosition() const;
    void onAds();

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<GameObject> mPlayer;
    //カメラ回転速度
    float mRotateSpeed;
    //プレイヤーまでの距離
    float mToPlayerDistance;
    //注視点のY軸オフセット
    float mPlayerHeadPointY;
    //ADS時のカメラ値
    Vector3 mAdsPosition;
    //カメラ回転時に反転するか
    bool mIsInverseX;
    bool mIsInverseY;
    //位置計算を行うか
    bool mIsCalcPosition;
};
