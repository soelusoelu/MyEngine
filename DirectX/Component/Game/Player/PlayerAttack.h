#pragma once

#include "../../Component.h"
#include <memory>

class SkinMeshComponent;

class PlayerAttack
    : public Component
{
public:
    PlayerAttack(GameObject& gameObject);
    ~PlayerAttack();
    virtual void start() override;
    virtual void update() override;

private:
    PlayerAttack(const PlayerAttack&) = delete;
    PlayerAttack& operator=(const PlayerAttack&) = delete;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
};
