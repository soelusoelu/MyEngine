#pragma once

#include "../../Component.h"
#include <memory>

class SkinMeshComponent;

class PlayerGuard
    : public Component
{
public:
    PlayerGuard();
    ~PlayerGuard();
    virtual void start() override;
    virtual void update() override;
    void originalUpdate();
    bool isGuarding() const;

private:
    PlayerGuard(const PlayerGuard&) = delete;
    PlayerGuard& operator=(const PlayerGuard&) = delete;

private:
    std::shared_ptr<SkinMeshComponent> mAnimation;
    bool mIsGuarding;
};
