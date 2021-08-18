#pragma once

#include "../PlayerEnemyCommon/IShotRaySetter.h"
#include "../../Component.h"
#include "../../../Collision/Collision.h"
#include "../../../Device/Function.h"
#include <functional>
#include <memory>

class GameObject;
class Camera;
class SkinMeshComponent;

class BulletShooter
    : public Component
{
public:
    BulletShooter();
    ~BulletShooter();
    BulletShooter(const BulletShooter&) = delete;
    BulletShooter& operator=(const BulletShooter&) = delete;

    virtual void start() override;
    virtual void update() override;

    void setConnector(const GameObject& connector);
    void ads();
    bool isAds() const;
    void onAds(const std::function<void()>& f);
    void onStartAds(const std::function<void()>& f);
    void onStopAds(const std::function<void()>& f);

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<SkinMeshComponent> mAnimation;
    Ray mShotRay;
    IShotRaySetter* mShotRaySetter;
    Function<void()> mOnAds;
    Function<void()> mOnStartAds;
    Function<void()> mOnStopAds;
    bool mIsADS;
};
