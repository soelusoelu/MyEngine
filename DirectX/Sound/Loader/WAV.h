#pragma once

#include "ISoundLoader.h"

class WAV : public ISoundLoader {
public:
    WAV();
    ~WAV();
    virtual void loadFromFile(std::shared_ptr<WaveformData>* data, const std::string& fileName) const override;
};
