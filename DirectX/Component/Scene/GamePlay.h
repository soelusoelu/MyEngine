﻿#pragma once

#include "../Component.h"
#include <memory>

class Scene;

class GamePlay : public Component {
public:
    GamePlay();
    ~GamePlay();
    virtual void start() override;
    virtual void update() override;

private:
    std::shared_ptr<Scene> mScene;
};
