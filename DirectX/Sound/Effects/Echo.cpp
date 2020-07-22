#include "Echo.h"
#include "../../DebugLayer/Debug.h"

Echo::Echo() :
    mXAPO(nullptr) {
}

Echo::~Echo() = default;

bool Echo::create(XAUDIO2_EFFECT_DESCRIPTOR* desc) {
    auto res = CreateFX(__uuidof(FXEcho), &mXAPO);
    if (FAILED(res)) {
        Debug::logError("Failed created echo.");
        return false;
    }

    desc->pEffect = mXAPO;

    return true;
}
