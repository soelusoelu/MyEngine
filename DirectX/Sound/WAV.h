#pragma once

#include "SoundLoader.h"

class WAV : public SoundLoader {
public:
    WAV();
    ~WAV();
    virtual void loadFromFile(const std::string& fileName) override;
};
