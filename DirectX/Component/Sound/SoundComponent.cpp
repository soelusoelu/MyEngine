#include "SoundComponent.h"
#include "../../Device/AssetsManager.h"
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
        mSound = World::instance().assetsManager().createSound(mFileName, param);
    }
    if (mIsFirstPlay) {

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

SoundPlayer& SoundComponent::getSoundPlayer() const {
    return mSound->getSoundPlayer();
}

SoundVolume& SoundComponent::getSoundVolume() const {
    return mSound->getSoundVolume();
}
