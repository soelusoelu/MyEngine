#include "SubmixVoiceComponent.h"
#include "../../Device/AssetsManager.h"
#include "../../Device/SoundCreater.h"
#include "../../Sound/Voice/SubmixVoice/SubmixVoice.h"
#include "../../Sound/Voice/SubmixVoice/SubmixVoiceInitParam.h"
#include "../../System/World.h"

SubmixVoiceComponent::SubmixVoiceComponent() :
    Component(),
    mSubmixVoice(nullptr) {
}

SubmixVoiceComponent::~SubmixVoiceComponent() = default;

void SubmixVoiceComponent::awake() {
    SubmixVoiceInitParam param;
    param.channels = 2;
    param.inputSampleRate = 44100;
    //mSubmixVoice = World::instance().assetsManager().getSoundCreater().createSubmixVoice(param);
}

bool SubmixVoiceComponent::isNull() const {
    return (mSubmixVoice) ? false : true;
}

SubmixVoice& SubmixVoiceComponent::getSubmixVoice() const {
    return *mSubmixVoice;
}

SoundEffect& SubmixVoiceComponent::getSoundEffect() const {
    return mSubmixVoice->getSoundEffect();
}
