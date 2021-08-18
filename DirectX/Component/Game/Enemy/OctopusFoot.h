#pragma once

#include "IOctopusPart.h"
#include "../../Component.h"
#include "../../../Device/Function.h"
#include <functional>
#include <memory>
#include <vector>

class MeshComponent;

class OctopusFoot
    : public Component
    , public IOctopusPart
{
public:
    OctopusFoot();
    ~OctopusFoot();
    OctopusFoot(const OctopusFoot&) = delete;
    OctopusFoot& operator=(const OctopusFoot&) = delete;

    virtual void start() override;
    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    virtual const OBBColliderPtrArray& getColliders() const override;
    virtual void takeDamage(int damage = 1) override;
    virtual int getHp() const override;
    virtual bool isDestroy() const override;

    //識別番号を取得する
    int getNumber() const;
    //足が死んだら呼ばれる
    void onDestroyFoot(const std::function<void(const OctopusFoot&)>& f);

private:
    //足が死んだら
    void destroyFoot();
    //色を赤くしていく
    void changeColor();

private:
    std::shared_ptr<MeshComponent> mMesh;
    OBBColliderPtrArray mColliders;
    std::vector<int> mTargetBoneNo;
    Function<void(const OctopusFoot&)> mOnDestroyFoot;
    int mFootMeshNumber;
    int mHp;
};
