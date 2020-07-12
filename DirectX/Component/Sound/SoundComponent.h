#pragma once

#include "../Component.h"
#include "../../Sound/SoundBuffer.h"
#include <string>

class SourceVoice;
class VoiceDetails;
class SoundPlayer;
class SoundVolume;
class SoundFilter;

class SoundComponent : public Component {
public:
    SoundComponent();
    ~SoundComponent();
    virtual void awake() override;
    virtual void update() override;
    virtual void finalize() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

    SourceVoice& getSourceVoice() const;
    SoundBuffer& getSoundBuffer() const;
    VoiceDetails& getSoundData() const;
    SoundPlayer& getSoundPlayer() const;
    SoundVolume& getSoundVolume() const;
    SoundFilter& getSoundFilter() const;

private:
    std::unique_ptr<SourceVoice> mSound;
    std::string mFileName;
    bool mIsFirstPlay;
};
