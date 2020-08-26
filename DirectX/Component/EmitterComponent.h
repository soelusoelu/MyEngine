#pragma once

#include "Component.h"
#include <memory>

class SoundComponent;

class EmitterComponent : public Component {
public:
    EmitterComponent(GameObject& gameObject);
    ~EmitterComponent();
    virtual void start() override;
    virtual void update() override;

private:
    EmitterComponent(const EmitterComponent&) = delete;
    EmitterComponent& operator=(const EmitterComponent&) = delete;

private:
    std::shared_ptr<SoundComponent> mSound;
};
