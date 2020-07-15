#include "OutputVoices.h"
#include "../../DebugLayer/Debug.h"

OutputVoices::OutputVoices(IVoice& voice) :
    mVoice(voice),
    mHasChangedDescs(false) {
}

OutputVoices::~OutputVoices() = default;

void OutputVoices::update() {
    if (mHasChangedDescs) {
        apply();
        mHasChangedDescs = false;
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

void OutputVoices::apply() {
    XAUDIO2_VOICE_SENDS sends;
    sends.SendCount = mDescs.size();
    sends.pSends = mDescs.data();
    auto res = mVoice.getXAudio2Voice()->SetOutputVoices(&sends);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed set output voices.");
    }
#endif // _DEBUG
}
