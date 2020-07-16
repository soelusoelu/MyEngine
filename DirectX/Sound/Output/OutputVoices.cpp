#include "OutputVoices.h"
#include "../../DebugLayer/Debug.h"
#include <cassert>

OutputVoices::OutputVoices(IVoice& voice) :
    mVoice(voice),
    mHasChangedDescs(false) {
}

OutputVoices::~OutputVoices() = default;

void OutputVoices::update() {
    if (mHasChangedDescs) {
        apply();
    }
}

void OutputVoices::setOutputVoices(const std::vector<std::shared_ptr<IVoice>>& voices, bool useFilter) {
    if (voices.empty()) {
        Debug::logWarning("Output voices is empty.");
        return;
    }

    const size_t voiceSize = voices.size();
    mDescs.resize(voiceSize);
    for (size_t i = 0; i < voiceSize; i++) {
        mDescs[i].Flags = (useFilter) ? XAUDIO2_SEND_USEFILTER : 0;
        mDescs[i].pOutputVoice = voices[i]->getXAudio2Voice();
    }

    mHasChangedDescs = true;
}

void OutputVoices::addOutputVoice(IVoice& voice, bool useFilter) {
    XAUDIO2_SEND_DESCRIPTOR desc;
    desc.Flags = (useFilter) ? XAUDIO2_SEND_USEFILTER : 0;
    desc.pOutputVoice = voice.getXAudio2Voice();
    mDescs.emplace_back(desc);

    mHasChangedDescs = true;
}

const XAUDIO2_SEND_DESCRIPTOR& OutputVoices::getDesc(unsigned index) const {
    assert(index < mDescs.size());
    return mDescs[index];
}

size_t OutputVoices::size() const {
    return mDescs.size();
}

void OutputVoices::apply() {
    if (mDescs.size() == 0) {
        Debug::logWarning("Descriptors is empty.");
        return;
    }

    XAUDIO2_VOICE_SENDS sends;
    sends.SendCount = mDescs.size();
    sends.pSends = mDescs.data();
    auto res = mVoice.getXAudio2Voice()->SetOutputVoices(&sends);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed set output voices.");
    }
#endif // _DEBUG

    mHasChangedDescs = false;
}
