#pragma once

#include "EnemyAlias.h"
#include "OctopusConstantNumbers.h"
#include "../../Component.h"
#include <array>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

class SkinMeshComponent;
class OctopusFootManager;
class Time;

class OctopusFootAttack
    : public Component
{
public:
    OctopusFootAttack();
    ~OctopusFootAttack();
    OctopusFootAttack(const OctopusFootAttack&) = delete;
    OctopusFootAttack& operator=(const OctopusFootAttack&) = delete;

    virtual void start() override;
    virtual void update() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;

    void attack();
    bool isAttacking() const;

private:
    //攻撃の種類を選択し、モーション番号を返す
    unsigned choiceAttack() const;
    //両足攻撃が可能か
    bool canDoubleAttack(const AliveNumbers& numbers, unsigned choiceNo) const;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    std::shared_ptr<OctopusFootManager> mFootManager;
    //叩きつけ攻撃時のペアとなる足配列
    std::array<std::pair<unsigned, unsigned>, OctopusConstantNumbers::FOOT_COUNT / 2> mFootPairs;
    //攻撃モーションマップ
    std::unordered_map<unsigned, unsigned> mSingleFootAttackMotionMap;
    std::unordered_map<unsigned, unsigned> mDoubleFootAttackMotionMap;
    //現在の攻撃モーションの時間
    std::unique_ptr<Time> mCurrentMotionTimer;
    //全攻撃モーション時間
    std::vector<float> mMotionsTime;
    //叩きつけ攻撃の発生確率
    int mSwingDownProbability;
    //攻撃中か
    bool mIsAttacking;
};
