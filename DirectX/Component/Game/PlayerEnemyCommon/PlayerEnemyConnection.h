#pragma once

#include "IShotRaySetter.h"
#include "../Enemy/IOctopusPart.h"
#include "../../Component.h"
#include "../../../Collision/Collision.h"
#include <memory>

class GameObject;
class Octopus;

class PlayerEnemyConnection
    : public Component
    , public IShotRaySetter
{
public:
    PlayerEnemyConnection();
    ~PlayerEnemyConnection();
    PlayerEnemyConnection(const PlayerEnemyConnection&) = delete;
    PlayerEnemyConnection& operator=(const PlayerEnemyConnection&) = delete;

    virtual void lateUpdate() override;

    virtual void setShotRay(const Ray* ray) override;

    void setEnemy(const GameObject& enemy);

private:
    //レイとタコの衝突判定
    void intersectRayOctopus() const;
    //レイとタコの足の衝突判定
    void intersectRayOctopusFoots() const;
    //レイとタコの頭の衝突判定
    void intersectRayOctopusHead() const;
    //レイとタコの部位の衝突判定
    void intersectRayOctopusPart(IOctopusPart* part) const;

private:
    std::shared_ptr<Octopus> mOctopus;
    //銃を撃った際のレイ
    const Ray* mShotRay;
};
