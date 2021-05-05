#pragma once

#include "../../Component.h"
#include <memory>

class SkinMeshComponent;
class Time;

class PlayerAttack
    : public Component
{
public:
    PlayerAttack();
    ~PlayerAttack();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void originalUpdate();
    //攻撃中か
    bool isAttacking() const;

private:
    PlayerAttack(const PlayerAttack&) = delete;
    PlayerAttack& operator=(const PlayerAttack&) = delete;

    //攻撃終了中更新
    void updateEndAttack();
    //1回目の攻撃開始
    void firstAttack();
    //2回目の攻撃開始
    void secondAttack();
    //攻撃終了
    void attackEnd();
    //1回目の攻撃終了
    void firstAttackEnd();
    //2回目の攻撃終了
    void secondAttackEnd();
    //モーションタイマー更新
    bool updateTimer();
    //1回目の攻撃が可能か
    bool canFirstAttack() const;
    //2回目の攻撃が可能か
    bool canSecondAttack() const;
    //1回目の攻撃が終わるか
    bool isEndFirstAttack() const;
    //2回目の攻撃が終わるか
    bool isEndSecondAttack() const;

private:
    static constexpr unsigned FIRST_ATTACK_START_NO = 0;
    static constexpr unsigned SECOND_ATTACK_START_NO = 1;
    static constexpr unsigned FIRST_ATTACK_END_NO = 2;
    static constexpr unsigned SECOND_ATTACK_END_NO = 3;
    static constexpr unsigned NUM_ATTACK_MOTION = 2;
    static constexpr unsigned NUM_ATTACK_MOTION_DOUBLE = NUM_ATTACK_MOTION * 2;

    std::shared_ptr<SkinMeshComponent> mAnimation;
    //モーション経過時間
    std::unique_ptr<Time> mAttackMotionElapsedTimer;
    //各攻撃モーション時間
    float mAttackMotionTime[NUM_ATTACK_MOTION_DOUBLE];
    //追加攻撃までの最低クールタイム
    float mLowestCoolTimeUpToAdditionalAttack[NUM_ATTACK_MOTION];
    //各攻撃の最中か
    bool mIsFirstAttackMiddle;
    bool mIsSecondAttackMiddle;
    //攻撃終了中か
    bool mIsEndAttackMiddle;
};
