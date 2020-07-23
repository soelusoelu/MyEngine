#pragma once

#include "../ISoundEffect.h"
#include <xapofx.h>

class Equalizer : public ISoundEffect {
public:
    Equalizer();
    ~Equalizer();
    virtual bool create(XAUDIO2_EFFECT_DESCRIPTOR* desc) override;

private:
    Equalizer(const Equalizer&) = delete;
    Equalizer& operator=(const Equalizer&) = delete;

private:
    IUnknown* mXAPO;
};
