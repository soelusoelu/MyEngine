#pragma once

#include "../../Component.h"
#include "../../../Device/Function.h"
#include <functional>
#include <memory>

class HitPoint
    : public Component
{
public:
    HitPoint();
    ~HitPoint();
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;
    virtual void drawInspector() override;

    //ダメージに応じてHPを減らす
    void takeDamage(int damage);
    //ヒール量に応じてHPを増やす
    void takeHeal(int heal);
    //HPを設定する
    void setHp(int hp, bool isChangeMax = true);
    //現在HPを取得する
    int getHp() const;
    //最大HPを取得する
    int getMaxHp() const;
    //最大HPに対しての現在HPの比率を取得する[0, 1]
    float getHpRate() const;
    //HPが変更された際のコールバック
    void callbackChangeHp(const std::function<void(const HitPoint&)>& callback);

private:
    HitPoint(const HitPoint&) = delete;
    HitPoint& operator=(const HitPoint&) = delete;

    //HPが最大HPを越したら調整
    void clampHpIfOverMax();

private:
    int mHp;
    int mMaxHp;
    Function<void(const HitPoint&)> mCallbackUpdateHp;
};
