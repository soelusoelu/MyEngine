#pragma once

#include "../../Component.h"
#include <memory>

class SkinMeshComponent;

class PlayerGuard
    : public Component
{
public:
    PlayerGuard(GameObject& gameObject);
    ~PlayerGuard();
    virtual void start() override;
    virtual void update() override;

private:
    PlayerGuard(const PlayerGuard&) = delete;
    PlayerGuard& operator=(const PlayerGuard&) = delete;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
};
