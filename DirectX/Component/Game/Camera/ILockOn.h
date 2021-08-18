#pragma once

#include <functional>

class Transform3D;

class ILockOn {
public:
    ~ILockOn() = default;
    //ロックオンしているか
    virtual bool isLockOn() const = 0;
    //ロックオン中のターゲットのトランスフォームを取得する
    virtual const Transform3D& getLockOnTargetTransform() const = 0;
    //ロックオンした際のコールバック
    virtual void callbackLockOn(const std::function<void()>& callback) = 0;
    //ロックオンを外した際のコールバック
    virtual void callbackUnlockOn(const std::function<void()>& callback) = 0;
};
