#pragma once

#include "../../Component.h"
#include <memory>

class SkinMeshComponent;
class Time;

class PlayerAttack
    : public Component
{
    struct AdditionalAttackTimer {
        //追加攻撃までの最低クールタイム
        float lowestCoolTimeUpToAdditionalAttack;
        //追加攻撃までの受付時間
        float receptionTimeUpToAdditionalAttack;
    };

public:
    PlayerAttack(GameObject& gameObject);
    ~PlayerAttack();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

private:
    PlayerAttack(const PlayerAttack&) = delete;
    PlayerAttack& operator=(const PlayerAttack&) = delete;

    void updateAttack();
    void firstAttack();
    void secondAttack();
    void attackEnd();
    //void continuousFirstAttack();
    bool isFirstAttackable() const;
    bool isSecondAttackable() const;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    //攻撃モーション経過時間
    std::unique_ptr<Time> mAttackMotionElapsedTime;
    //追加攻撃まで時間
    AdditionalAttackTimer mAdditionalAttackTimer[2];
    //各攻撃の最中か
    bool mIsFirstAttackMiddle;
    bool mIsSecondAttackMiddle;
};
