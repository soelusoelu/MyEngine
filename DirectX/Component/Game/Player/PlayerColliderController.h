#pragma once

#include "../../Component.h"
#include <memory>

class AABBAnimationCollider;

class PlayerColliderController
    : public Component
{
public:
    PlayerColliderController();
    ~PlayerColliderController();
    virtual void start() override;

private:
    PlayerColliderController(const PlayerColliderController&) = delete;
    PlayerColliderController& operator=(const PlayerColliderController&) = delete;

private:
    std::shared_ptr<AABBAnimationCollider> mCollider;

    static constexpr unsigned HEAD = 0;
    static constexpr unsigned NOSE = 2;
    static constexpr unsigned LEFT_EYE = 5;
    static constexpr unsigned RIGHT_EYE = 6;
};
