#include "SoundPan.h"
#include "../Output/OutputVoices.h"
#include "../Voice/VoiceDetails.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"
#include "../../System/Window.h"
#include <vector>

SoundPan::SoundPan(IVoice& voice, const MasteringVoice& masteringVoice, OutputVoices& outputVoices) :
    mVoice(voice),
    mOutputVoices(outputVoices),
    INPUT_CHANNELS(voice.getVoiceDetails().channels),
    OUTPUT_CHANNELS(masteringVoice.getDetails().channels) {
}

SoundPan::~SoundPan() = default;

void SoundPan::pan(float positionX, unsigned operationSet) {
    const float width = static_cast<float>(Window::standardWidth());

    auto posX = Math::clamp<float>(positionX, 0.f, width);
    float rot = posX / width * 90.f;
    std::vector<float> volumes(INPUT_CHANNELS * OUTPUT_CHANNELS);
    volumes[0] = volumes[1] = Math::cos(rot);
    volumes[2] = volumes[3] = Math::sin(rot);

    selectOutput(volumes.data(), operationSet);
}

void SoundPan::panCenter(unsigned operationSet) {
    static constexpr float centerVolume = 0.707f;
    std::vector<float> volumes(INPUT_CHANNELS * OUTPUT_CHANNELS, centerVolume);

    selectOutput(volumes.data(), operationSet);
}

void SoundPan::selectOutput(const float volumes[], unsigned operationSet) {
    auto descSize = mOutputVoices.size();
    if (descSize == 0) {
        setOutputMatrix(nullptr, volumes, operationSet);
    } else {
        for (size_t i = 0; i < descSize; i++) {
            const auto& desc = mOutputVoices.getDesc(i);
            setOutputMatrix(desc.pOutputVoice, volumes, operationSet);
        }
    }
}

void SoundPan::setOutputMatrix(IXAudio2Voice* outputVoice, const float volumes[], unsigned operationSet) {
    auto res = mVoice.getXAudio2Voice()->SetOutputMatrix(outputVoice, INPUT_CHANNELS, OUTPUT_CHANNELS, volumes, operationSet);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed pan.");
    }
#endif // _DEBUG
}
