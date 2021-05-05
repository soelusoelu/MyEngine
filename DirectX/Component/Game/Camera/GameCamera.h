#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include <memory>

class Camera;

class GameCamera
    : public Component
{
public:
    GameCamera();
    ~GameCamera();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

    //カメラの注視点となるプレイヤーを設定する
    void setPlayer(const std::shared_ptr<GameObject>& player);

private:
    GameCamera(const GameCamera&) = delete;
    GameCamera& operator=(const GameCamera&) = delete;

private:
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<GameObject> mPlayer;
    //カメラ回転速度
    float mRotateSpeed;
    //プレイヤーまでの距離
    float mToPlayerDistance;
    //注視点のY軸オフセット
    float mLookAtOffsetY;
};
