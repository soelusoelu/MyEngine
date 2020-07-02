#pragma once

#include "../Component.h"
#include <string>

class Sound;

class SoundComponent : public Component {
public:
    SoundComponent();
    ~SoundComponent();
    virtual void awake() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void play();
    void play(const std::string& fileName, float volumeScale = 1.f);
    bool isFinished() const;
    void setVolume(float volume);
    void setVolumeByDecibels(float decibels);

private:
    std::shared_ptr<Sound> mSound;
    std::string mFileName;
    float mVolume;
    bool mIsFirstPlay;
};
