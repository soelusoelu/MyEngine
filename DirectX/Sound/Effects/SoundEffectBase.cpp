#include "SoundEffectBase.h"
#include "../../DebugLayer/Debug.h"

SoundEffectBase::SoundEffectBase(IVoice& voice, MasteringVoice& masteringVoice) :
    mVoice(voice),
    mMasteringVoice(masteringVoice),
    mXAPO(nullptr) {
}

SoundEffectBase::~SoundEffectBase() = default;
