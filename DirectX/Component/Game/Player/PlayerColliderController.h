#pragma once

#include "../../Component.h"
#include <memory>

class HitPoint;

class PlayerColliderController
    : public Component
{
public:
    PlayerColliderController();
    ~PlayerColliderController();
    virtual void start() override;
    virtual void onCollisionEnter(Collider& other) override;

private:
    PlayerColliderController(const PlayerColliderController&) = delete;
    PlayerColliderController& operator=(const PlayerColliderController&) = delete;

private:
    std::shared_ptr<HitPoint> mHP;
};
