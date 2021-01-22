﻿#pragma once

#include "../../Input/IMouse.h"
#include "../../Math/Math.h"
#include <memory>

class SimpleCamera;

//エンジンのデフォルトで使われるカメラ
class EngineCamera {
public:
    EngineCamera();
    ~EngineCamera();
    void update();
    SimpleCamera& getCamera() const;

private:
    EngineCamera(const EngineCamera&) = delete;
    EngineCamera& operator=(const EngineCamera&) = delete;

    //カメラを移動する
    void moveCamera(const IMouse& mouse, const Vector2& mouseMoveAmount);
    //注視点を中心にカメラを回転する
    void rotateLookAtPoint(const IMouse& mouse, const Vector2& mouseMoveAmount);
    //マウスの移動量から回転軸を計算する
    void computeRotation(const Vector2& mouseMoveAmount);

    //カメラから注視点までのベクトルを求める
    Vector3 getCameraToLookAt() const;
    //カメラから注視点までの距離を計算する
    void computeLengthCameraToLookAt();

    //カメラを移動できる状態か
    bool canMove(const IMouse& mouse, const Vector2& mouseMoveAmount) const;
    //カメラを回転できる状態か
    bool canRotate(const IMouse& mouse, const Vector2& mouseMoveAmount) const;
    //マウスが移動したか
    bool isMoveMousePosition(const Vector2& mouseMoveAmount) const;

private:
    std::unique_ptr<SimpleCamera> mCamera;
    Quaternion mCameraRotation;
    float mLengthCameraToLookAt;

    static constexpr float MOVE_SPEED = 0.0065f;
    static constexpr float ROTATE_SPEED = 0.5f;
};
