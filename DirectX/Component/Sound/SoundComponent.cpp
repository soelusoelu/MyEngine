#include "SoundComponent.h"
#include "../../Device/AssetsManager.h"
#include "../../Device/Flag.h"
#include "../../Sound/SoundFlag.h"
#include "../../Sound/SoundPlayer.h"
#include "../../Sound/SourceVoice.h"
#include "../../Sound/SourceVoiceInitParam.h"
#include "../../System/World.h"
#include "../../Utility/LevelLoader.h"

SoundComponent::SoundComponent() :
    Component(),
    mSound(nullptr),
    mFileName(""),
    mIsFirstPlay(false) {
}

SoundComponent::~SoundComponent() = default;

void SoundComponent::awake() {
    if (!mFileName.empty()) {
        SourceVoiceInitParam param;
        param.flags.set(static_cast<unsigned>(SoundFlag::USE_FILTER));
        //param.maxFrequencyRatio = 16.f;
        mSound = World::instance().assetsManager().createSound(mFileName, param);
    }
    if (mIsFirstPlay) {
        //mSound->getSoundPlayer().play();
    }
}

void SoundComponent::update() {
    mSound->update();
}

void SoundComponent::finalize() {
    if (mSound) {
        mSound->getSoundPlayer().stop();
    }
}

void SoundComponent::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getString(inObj, "fileName", &mFileName);
    //JsonHelper::getFloat(inObj, "volume", &mVolume);
    JsonHelper::getBool(inObj, "isFirstPlay", &mIsFirstPlay);
}

void SoundComponent::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("FileName", mFileName);
}

SoundBuffer& SoundComponent::getSoundBuffer() const {
    return mSound->getSoundBuffer();
}

SoundPlayer& SoundComponent::getSoundPlayer() const {
    return mSound->getSoundPlayer();
}

SoundVolume& SoundComponent::getSoundVolume() const {
    return mSound->getSoundVolume();
}

SoundFilter& SoundComponent::getSoundFilter() const {
    return mSound->getSoundFilter();
}
