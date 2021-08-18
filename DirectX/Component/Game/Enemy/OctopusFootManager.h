#pragma once

#include "EnemyAlias.h"
#include "../../Component.h"
#include <memory>
#include <vector>

class OctopusFoot;

class OctopusFootManager
    : public Component
{
public:
    OctopusFootManager();
    ~OctopusFootManager();
    OctopusFootManager(const OctopusFootManager&) = delete;
    OctopusFootManager& operator=(const OctopusFootManager&) = delete;

    virtual void start() override;

    //足をすべて取得する
    const OctopusFootPtrArray& getFoots() const;
    //生きている足の番号をすべて取得する
    const AliveNumbers& getAliveFootNumbers() const;
    //一本でも足があるか
    bool isFootAlive() const;

private:
    //管理中の足が死んだら呼ばれる
    void onDestroyFoot(const OctopusFoot& foot);

private:
    OctopusFootPtrArray mFoots;
    AliveNumbers mFootAliveNumbers;
};
