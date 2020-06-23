#pragma once

#include "Component.h"
#include <string>

class Sound;

class SoundComponent : public Component {
public:
    SoundComponent();
    ~SoundComponent();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void playBGM();
    void playBGM(const std::string& fileName, float volumeScale = 1.f);
    void playSE();
    void playSE(const std::string& fileName, float volumeScale = 1.f);

private:
    std::shared_ptr<Sound> mSound;
    std::string mFileName;
    float mVolume;
    bool mIsFirstPlay;
};
