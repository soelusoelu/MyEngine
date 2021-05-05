#pragma once

#include "../../Engine/Scene/Scene.h"

class Title : public Scene {
public:
    Title();
    ~Title();
    virtual void awake() override;

private:
    Title(const Title&) = delete;
    Title& operator=(const Title&) = delete;
};
