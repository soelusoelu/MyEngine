#pragma once

#include "Component.h"
#include <memory>

class SoundComponent;

class EmitterComponent : public Component {
public:
    EmitterComponent();
    ~EmitterComponent();
    virtual void update() override;
    virtual void start() override;

private:
    EmitterComponent(const EmitterComponent&) = delete;
    EmitterComponent& operator=(const EmitterComponent&) = delete;

private:
    std::shared_ptr<SoundComponent> mSound;
};
