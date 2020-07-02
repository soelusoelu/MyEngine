#pragma once

#include "../Component.h"
#include <string>

class SourceVoice;
class SoundPlayer;
class SoundVolume;

class SoundComponent : public Component {
public:
    SoundComponent();
    ~SoundComponent();
    virtual void awake() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

    SoundPlayer& getSoundPlayer() const;
    SoundVolume& getSoundVolume() const;

private:
    std::shared_ptr<SourceVoice> mSound;
    std::string mFileName;
    bool mIsFirstPlay;
};
