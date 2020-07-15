#pragma once

#include "../Component.h"
#include <memory>

class SubmixVoice;
class SoundEffect;

class SubmixVoiceComponent : public Component {
public:
    SubmixVoiceComponent();
    ~SubmixVoiceComponent();
    virtual void awake() override;

    //使用可能状態か
    bool isNull() const;

    SubmixVoice& getSubmixVoice() const;
    SoundEffect& getSoundEffect() const;

private:
    std::unique_ptr<SubmixVoice> mSubmixVoice;
};
