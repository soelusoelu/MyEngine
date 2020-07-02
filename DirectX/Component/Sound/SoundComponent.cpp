#include "SoundComponent.h"
#include "../../Device/AssetsManager.h"
#include "../../Sound/Sound.h"
#include "../../System/World.h"
#include "../../Utility/LevelLoader.h"

SoundComponent::SoundComponent() :
    Component(),
    mSound(std::make_shared<Sound>()),
    mFileName(""),
    mVolume(1.f),
    mLastVolume(0.f),
    mIsFirstPlay(false) {
}

SoundComponent::~SoundComponent() {
    if (mSound) {
        mSound->stop();
    }
}

void SoundComponent::awake() {
    if (mIsFirstPlay) {

    }
    if (!mFileName.empty()) {
        World::instance().assetsManager().createSound(&mSound, mFileName);
    }
}

void SoundComponent::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getString(inObj, "fileName", &mFileName);
    JsonHelper::getFloat(inObj, "volume", &mVolume);
    JsonHelper::getBool(inObj, "isFirstPlay", &mIsFirstPlay);
}

void SoundComponent::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("FileName", mFileName);
    inspect->emplace_back("Volume", mVolume);
    inspect->emplace_back("IsFirstPlay", mIsFirstPlay);
}

void SoundComponent::play() {
    //playSE(mFileName, mVolume);
    World::instance().assetsManager().createSound(&mSound, mFileName);

    mSound->setVolume(mVolume);
    mSound->play();
}

void SoundComponent::play(const std::string& fileName, float volumeScale) {
    auto sound = std::make_shared<Sound>();
    World::instance().assetsManager().createSound(&sound, fileName);

    sound->setVolume(volumeScale);
    sound->play();
}

bool SoundComponent::isFinished() const {
    if (!mSound) {
        return false;
    }
    return mSound->isFinished();
}

void SoundComponent::setVolume(float volume) {
    mSound->setVolume(volume);
}

void SoundComponent::setVolumeByDecibels(float decibels) {
    mSound->setVolumeByDecibels(decibels);
}

void SoundComponent::fade(float targetVolume, float targetTime) {

}
