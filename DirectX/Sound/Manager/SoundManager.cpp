#include "SoundManager.h"
#include "../3D/Sound3DCalculator.h"
#include "../3D/Listener/Sound3DListener.h"
#include "../Voice/SourceVoice/SourceVoice.h"

SoundManager::SoundManager(const MasteringVoice& masteringVoice) :
    mCalculator(std::make_unique<Sound3DCalculator>(masteringVoice)),
    mListener(nullptr)
{
}

SoundManager::~SoundManager() = default;

void SoundManager::update() {
    if (mListener) {
        mListener->update();
    }

    for (const auto& sound : mSounds) {
        sound->update();

        if (sound->isCalculate3D() && mListener) {
            mCalculator->calculate(mListener->getListener(), sound->getEmitter());
        }
    }
}

void SoundManager::add(const SoundPtr& sound) {
    mSounds.emplace_back(sound);
}

void SoundManager::setListener(const std::shared_ptr<Sound3DListener>& listener) {
    mListener = listener;
}
