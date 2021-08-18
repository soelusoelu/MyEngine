#include "OctopusFootAttack.h"
#include "EnemyMotions.h"
#include "OctopusFootManager.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Time.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Utility/JsonHelper.h"
#include "../../../Utility/Random.h"
#include <algorithm>

OctopusFootAttack::OctopusFootAttack()
    : Component()
    , mAnimation(nullptr)
    , mFootManager(nullptr)
    , mCurrentMotionTimer(std::make_unique<Time>())
    , mMotionsTime(EnemyMotions::MOTION_COUNT)
    , mSwingDownProbability(0)
    , mIsAttacking(false)
{
    //両足攻撃時のペア形成
    mFootPairs[0] = std::make_pair(0, 1);
    mFootPairs[1] = std::make_pair(2, 7);
    mFootPairs[2] = std::make_pair(3, 6);
    mFootPairs[3] = std::make_pair(4, 5);

    //単足攻撃のマップ形成
    mSingleFootAttackMotionMap.emplace(0, EnemyMotions::HORIZONTAL_RIGHT_ONE);
    mSingleFootAttackMotionMap.emplace(1, EnemyMotions::HORIZONTAL_LEFT_ONE);
    mSingleFootAttackMotionMap.emplace(2, EnemyMotions::HORIZONTAL_LEFT_TWO);
    mSingleFootAttackMotionMap.emplace(3, EnemyMotions::HORIZONTAL_LEFT_THREE);
    mSingleFootAttackMotionMap.emplace(4, EnemyMotions::HORIZONTAL_LEFT_FOUR);
    mSingleFootAttackMotionMap.emplace(5, EnemyMotions::HORIZONTAL_RIGHT_FOUR);
    mSingleFootAttackMotionMap.emplace(6, EnemyMotions::HORIZONTAL_RIGHT_THREE);
    mSingleFootAttackMotionMap.emplace(7, EnemyMotions::HORIZONTAL_RIGHT_TWO);

    //二足攻撃のマップ形成
    mDoubleFootAttackMotionMap.emplace(0, EnemyMotions::VERTICAL_ONE);
    mDoubleFootAttackMotionMap.emplace(1, EnemyMotions::VERTICAL_ONE);
    mDoubleFootAttackMotionMap.emplace(2, EnemyMotions::VERTICAL_TWO);
    mDoubleFootAttackMotionMap.emplace(3, EnemyMotions::VERTICAL_THREE);
    mDoubleFootAttackMotionMap.emplace(4, EnemyMotions::VERTICAL_FOUR);
    mDoubleFootAttackMotionMap.emplace(5, EnemyMotions::VERTICAL_FOUR);
    mDoubleFootAttackMotionMap.emplace(6, EnemyMotions::VERTICAL_THREE);
    mDoubleFootAttackMotionMap.emplace(7, EnemyMotions::VERTICAL_TWO);
}

OctopusFootAttack::~OctopusFootAttack() = default;

void OctopusFootAttack::start() {
    mAnimation = getComponent<SkinMeshComponent>();
    mFootManager = getComponent<OctopusFootManager>();

    //全攻撃モーションの時間を求める
    for (unsigned i = 0; i < EnemyMotions::MOTION_COUNT; ++i) {
        const auto& motion = mAnimation->getMotion(i);
        mMotionsTime[i] = static_cast<float>(motion.numFrame) / 60.f;
    }
}

void OctopusFootAttack::update() {
    //攻撃中じゃなければ終了
    if (!isAttacking()) {
        return;
    }

    //タイマー更新
    mCurrentMotionTimer->update();
    if (mCurrentMotionTimer->isTime()) {
        mCurrentMotionTimer->reset();

        mAnimation->tPose();
    }
}

void OctopusFootAttack::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mSwingDownProbability, "swingDownProbability", inObj, alloc, mode);
}

void OctopusFootAttack::drawInspector() {
    ImGuiWrapper::sliderInt("Swing Down Probability", mSwingDownProbability, 0, 100);
}

void OctopusFootAttack::attack() {
    auto motionNo = choiceAttack();

    //アニメーション変更
    mAnimation->changeMotion(motionNo);
    mAnimation->setLoop(false);

    //時間設定(モーション番号と配列の添字が一致している前提)
    mCurrentMotionTimer->setLimitTime(mMotionsTime[motionNo]);
    mCurrentMotionTimer->reset();
}

bool OctopusFootAttack::isAttacking() const {
    //モーションが攻撃しかない前提
    return (mAnimation->getCurrentMotionNumber() != SkinMeshComponent::T_POSE_NO);
}

unsigned OctopusFootAttack::choiceAttack() const {
    //生きている足から攻撃モーションを選択する
    const auto& aliveFoots = mFootManager->getAliveFootNumbers();
    auto value = Random::randomRange(0, aliveFoots.size());
    auto choiceNo = aliveFoots[value];

    //両足を使う攻撃が可能か検証
    auto canDouble = canDoubleAttack(aliveFoots, choiceNo);

    //どの攻撃方法を取るか
    unsigned motionNo = 0;
    if (!canDouble) {
        motionNo = mSingleFootAttackMotionMap.at(choiceNo);
    } else {
        value = Random::randomRange(0, 100);
        if (value < mSwingDownProbability) {
            motionNo = mDoubleFootAttackMotionMap.at(choiceNo);
        } else {
            motionNo = mSingleFootAttackMotionMap.at(choiceNo);
        }
    }

    return motionNo;
}

bool OctopusFootAttack::canDoubleAttack(const AliveNumbers& numbers, unsigned choiceNo) const {
    //全ペアからしらみつぶしに探す
    for (const auto& pair : mFootPairs) {
        if (choiceNo == pair.first) {
            auto itr = std::find(numbers.begin(), numbers.end(), pair.second);
            if (itr != numbers.end()) {
                return true;
            }
        }
        if (choiceNo == pair.second) {
            auto itr = std::find(numbers.begin(), numbers.end(), pair.first);
            if (itr != numbers.end()) {
                return true;
            }
        }
    }

    //片方の足が欠損している
    return false;
}
