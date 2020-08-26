#pragma once

#include "Component.h"
#include <memory>

class SoundComponent;

class EmitterComponent2 : public Component {
public:
    EmitterComponent2(GameObject& gameObject);
    ~EmitterComponent2();
    virtual void start() override;
    virtual void update() override;

private:
    EmitterComponent2(const EmitterComponent2&) = delete;
    EmitterComponent2& operator=(const EmitterComponent2&) = delete;

private:
    std::shared_ptr<SoundComponent> mSound;
};
