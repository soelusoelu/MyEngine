#pragma once

#include "../../Engine/EngineComponentAlias.h"

//タコの部位インターフェース
class IOctopusPart {
public:
    virtual ~IOctopusPart() = default;
    //部位に付随するすべてのコライダーを取得する
    virtual const OBBColliderPtrArray& getColliders() const = 0;
    //部位にダメージを与える
    virtual void takeDamage(int damage = 1) = 0;
    //HPを取得する
    virtual int getHp() const = 0;
    //部位が破損しているか
    virtual bool isDestroy() const = 0;
};
