﻿#include "SoundComponent.h"
#include "../../../Imgui/imgui.h"
#include "../../../Sound/3D/Emitter/Sound3DEmitter.h"
#include "../../../Sound/Player/SoundPlayer.h"
#include "../../../Sound/Voice/SourceVoice/SourceVoice.h"
#include "../../../Sound/Voice/SourceVoice/SourceVoiceInitParam.h"
#include "../../../Sound/XAudio2/SoundEngine.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"

SoundComponent::SoundComponent() :
    Component(),
    mSound(nullptr),
    mFileName(""),
    mUse3DSound(false) {
}

SoundComponent::~SoundComponent() = default;

void SoundComponent::awake() {
    if (!mFileName.empty()) {
        SourceVoiceInitParam param;
        param.maxFrequencyRatio = 4.f;
        param.isCalculate3D = mUse3DSound;
        mSound = SoundEngine::instance().createSourceVoice(mFileName, param);
    }
}

void SoundComponent::update() {
    if (!mSound) {
        return;
    }
    if (!mUse3DSound) {
        return;
    }
    mSound->getEmitter().setPosition(transform().getPosition());
}

void SoundComponent::finalize() {
    if (mSound) {
        mSound->getSoundPlayer().stop();
    }
}

void SoundComponent::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getString(inObj, "fileName", &mFileName);
    JsonHelper::getBool(inObj, "use3D", &mUse3DSound);
}

void SoundComponent::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {
    JsonHelper::setString(alloc, inObj, "fileName", mFileName);
    JsonHelper::setBool(alloc, inObj, "use3D", mUse3DSound);
}

void SoundComponent::drawInspector() {
    ImGui::Text("FileName: &s", mFileName);
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
