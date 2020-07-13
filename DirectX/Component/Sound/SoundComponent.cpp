#include "SoundComponent.h"
#include "../../Device/AssetsManager.h"
#include "../../Device/Flag.h"
#include "../../Device/SoundCreater.h"
#include "../../Sound/Flag/SoundFlag.h"
#include "../../Sound/Player/SoundPlayer.h"
#include "../../Sound/Voice/SourceVoice/SourceVoice.h"
#include "../../Sound/Voice/SourceVoice/SourceVoiceInitParam.h"
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
        mSound = World::instance().assetsManager().getSoundCreater().createSourceVoice(mFileName, param);
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

SourceVoice& SoundComponent::getSourceVoice() const {
    return *mSound;
}

SoundBuffer& SoundComponent::getSoundBuffer() const {
    return mSound->getSoundBuffer();
}

VoiceDetails& SoundComponent::getSoundData() const {
    return mSound->getSoundData();
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
