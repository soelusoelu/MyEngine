#pragma once

#include "IPlayerMove.h"
#include "../../Component.h"
#include "../../../Device/Function.h"
#include <functional>
#include <memory>

class SkinMeshComponent;

class PlayerDash
    : public Component
{
public:
    PlayerDash();
    ~PlayerDash();
    virtual void start() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    //ダッシュ処理
    void dash(IPlayerMove& playerMove);
    //走っているか
    bool isDashing() const;
    //歩行に移行した際のコールバック
    void callbackToDash(const std::function<void()>& callback);
    //ダッシュでスタミナが尽きた際のコールバック
    void callbackRunOutOfStamina(const std::function<void()>& callback);

private:
    PlayerDash(const PlayerDash&) = delete;
    PlayerDash& operator=(const PlayerDash&) = delete;

    //モーション変更時のコールバック
    void onChangeMotion();

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    Function<void()> mCallbackToDash;
    Function<void()> mCallbackRunOutOfStamina;
    //ダッシュ速度
    float mDashSpeed;
    //ダッシュ中か
    bool mIsDashing;
};
