#include "SoundComponent.h"
#include "../../Device/AssetsManager.h"
#include "../../Device/Flag.h"
#include "../../Device/SoundCreater.h"
#include "../../Sound/3D/Sound3D.h"
#include "../../Sound/3D/Emitter/Sound3DEmitter.h"
#include "../../Sound/Flag/SoundFlag.h"
#include "../../Sound/Player/SoundPlayer.h"
#include "../../Sound/Voice/SourceVoice/SourceVoice.h"
#include "../../Sound/Voice/SourceVoice/SourceVoiceInitParam.h"
#include "../../System/World.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

SoundComponent::SoundComponent() :
    Component(),
    mSound(nullptr),
    mFileName("") {
}

SoundComponent::~SoundComponent() = default;

void SoundComponent::awake() {
    if (!mFileName.empty()) {
        SourceVoiceInitParam param;
        param.flags.set(SoundFlags::USE_FILTER);
        param.maxFrequencyRatio = 4.f;
        mSound = World::instance().assetsManager().getSoundCreater().createSourceVoice(mFileName, param);
    }
}

void SoundComponent::update() {
    if (mSound) {
        mSound->update();
    }
}

void SoundComponent::lateUpdate() {
    if (mSound) {
        mSound->get3DSound().getEmitter().setPosition(transform().getPosition());
        mSound->get3DSound().update();
    }
}

void SoundComponent::finalize() {
    if (mSound) {
        mSound->getSoundPlayer().stop();
    }
}

void SoundComponent::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getString(inObj, "fileName", &mFileName);
}

void SoundComponent::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("FileName", mFileName);
}

bool SoundComponent::isNull() const {
    return (mSound) ? false : true;
}

const VoiceDetails& SoundComponent::getVoiceDetails() const {
    return mSound->getVoiceDetails();
}

SoundData& SoundComponent::getSoundData() const {
    return mSound->getSoundData();
}

SoundPlayer& SoundComponent::getSoundPlayer() const {
    return mSound->getSoundPlayer();
}

SoundVolume& SoundComponent::getSoundVolume() const {
    return mSound->getSoundVolume();
}

OutputVoices& SoundComponent::getOutputVoices() const {
    return mSound->getOutputVoices();
}

SoundEffect& SoundComponent::getSoundEffect() const {
    return mSound->getSoundEffect();
}
