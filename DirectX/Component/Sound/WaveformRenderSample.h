#pragma once

#include "../Component.h"

class WaveformRenderSample : public Component {
public:
    WaveformRenderSample(GameObject& gameObject);
    ~WaveformRenderSample();
    virtual void start() override;
    virtual void update() override;

private:
};
