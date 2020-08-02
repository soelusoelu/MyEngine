#pragma once

#include "../Component.h"
#include "../../Sound/Data/SoundBuffer.h"
#include "../../Sound/Voice/IVoice.h"
#include "../../Sound/Voice/VoiceDetails.h"
#include <memory>
#include <string>
#include <vector>

class SourceVoice;
class SoundPlayer;
class SoundVolume;
class SoundEffect;
class OutputVoices;

class SoundComponent : public Component {
public:
    SoundComponent();
    ~SoundComponent();
    virtual void awake() override;
    virtual void update() override;
    virtual void finalize() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

    //使用可能状態か
    bool isNull() const;

    SoundBuffer& getSoundBuffer() const;
    const VoiceDetails& getVoiceDetails() const;
    SoundData& getSoundData() const;
    SoundPlayer& getSoundPlayer() const;
    SoundVolume& getSoundVolume() const;
    OutputVoices& getOutputVoices() const;
    SoundEffect& getSoundEffect() const;

private:
    std::unique_ptr<SourceVoice> mSound;
    std::string mFileName;
};
