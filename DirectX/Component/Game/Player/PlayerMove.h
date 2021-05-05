#pragma once

#include "../../Component.h"
#include "../../../Input/Input.h"
#include "../../../Math/Math.h"
#include <memory>

class Camera;
class SkinMeshComponent;
class Time;

class PlayerMove
    : public Component
{
    enum class State {
        IDOL,
        WALK,
        DASH
    };

public:
    PlayerMove();
    ~PlayerMove();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void originalUpdate();
    //動いているか
    bool isMoving() const;
    //歩いているか
    bool isWalking() const;
    //走っているか
    bool isDashing() const;

private:
    PlayerMove(const PlayerMove&) = delete;
    PlayerMove& operator=(const PlayerMove&) = delete;

    void walk(const Vector2& leftStickValue);
    void dash(const JoyPad& pad, const Vector2& leftStickValue);
    void move(float moveSpeed, const Vector2& leftStickValue);
    void stop();
    bool canMove(const Vector2& leftStickValue) const;
    bool canDash(const JoyPad& pad);
    void onChangeMotion();

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::unique_ptr<Time> mDashMigrationTimer;
    float mWalkSpeed;
    float mDashSpeed;
    bool mIsWalking;
    bool mIsDashing;
    static constexpr JoyCode DASH_BUTTON = JoyCode::B;
};
