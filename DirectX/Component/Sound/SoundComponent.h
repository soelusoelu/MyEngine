#pragma once

#include "../Component.h"
#include "../../Sound/SoundBuffer.h"
#include <string>

class SourceVoice;
class SoundPlayer;
class SoundVolume;

class SoundComponent : public Component {
public:
    SoundComponent();
    ~SoundComponent();
    virtual void awake() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

    SoundBuffer& getSoundBuffer() const;
    SoundPlayer& getSoundPlayer() const;
    SoundVolume& getSoundVolume() const;

private:
    std::shared_ptr<SourceVoice> mSound;
    std::string mFileName;
    bool mIsFirstPlay;
};
