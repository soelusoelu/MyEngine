#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include <memory>

class Camera;
class SkinMeshComponent;

class PlayerMove
    : public Component
{
public:
    PlayerMove(GameObject& gameObject);
    ~PlayerMove();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

private:
    PlayerMove(const PlayerMove&) = delete;
    PlayerMove& operator=(const PlayerMove&) = delete;

    void move(const Vector2& leftStickValue);
    void stop();
    bool isMovable(const Vector2& leftStickValue) const;

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<SkinMeshComponent> mAnimation;
    float mWalkSpeed;
    bool mIsMoving;
};
