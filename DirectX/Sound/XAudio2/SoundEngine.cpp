#include "SoundEngine.h"
#include "SoundBase.h"
#include "../3D/Sound3DCalculator.h"
#include "../Manager/SoundManager.h"
#include "../Voice/SourceVoice/SourceVoice.h"
#include "../Voice/SubmixVoice/SubmixVoice.h"
#include "../../Device/SoundCreater.h"
#include "../../System/GlobalFunction.h"

SoundEngine::SoundEngine() :
    mBase(std::make_unique<SoundBase>()),
    mCreater(std::make_unique<SoundCreater>(*mBase)),
    mManager(std::make_unique<SoundManager>(mBase->getMasteringVoice())) {
}

SoundEngine::~SoundEngine() = default;

SoundEngine& SoundEngine::instance() {
    if (!mInstance) {
        mInstance = new SoundEngine();
    }
    return *mInstance;
}

void SoundEngine::finalize() {
    safeDelete(mInstance);
}

void SoundEngine::update() {
    mManager->update();
}

std::shared_ptr<SourceVoice> SoundEngine::createSourceVoice(const std::string& filePath, const SourceVoiceInitParam& param) const {
    auto sourceVoice = mCreater->createSourceVoice(filePath, param);
    mManager->add(sourceVoice);
    return sourceVoice;
}

std::shared_ptr<SubmixVoice> SoundEngine::createSubmixVoice(const SubmixVoiceInitParam& param) const {
    return mCreater->createSubmixVoice(param);
}

const SoundBase& SoundEngine::getBase() const {
    return *mBase;
}

SoundManager& SoundEngine::getManager() const {
    return *mManager;
}
