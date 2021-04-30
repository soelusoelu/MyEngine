#pragma once

#include "../../Component.h"
#include <memory>

class SkinMeshComponent;
class Time;

class PlayerRoll
    : public Component
{
public:
    PlayerRoll(GameObject& gameObject);
    ~PlayerRoll();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

private:
    PlayerRoll(const PlayerRoll&) = delete;
    PlayerRoll& operator=(const PlayerRoll&) = delete;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    //ローリング距離
    float mRollingDistance;
    //ローリング中か
    bool mIsDuringRolling;
    //ローリングモーション時間
    std::unique_ptr<Time> mRollingMotionTime;
};
