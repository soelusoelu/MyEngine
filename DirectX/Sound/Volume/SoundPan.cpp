#include "SoundPan.h"
#include "../Voice/VoiceDetails.h"
#include "../Voice/Output/OutputVoices.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"
#include "../../System/Window.h"
#include <vector>

SoundPan::SoundPan(IVoice& voice, const unsigned inChannels, const unsigned outChannels) :
    mVoice(voice),
    INPUT_CHANNELS(inChannels),
    OUTPUT_CHANNELS(outChannels) {
}

SoundPan::~SoundPan() = default;

void SoundPan::pan(const float volumes[]) {
    selectOutput(volumes);
}

void SoundPan::panFromPositionX(float positionX) {
    const float width = static_cast<float>(Window::standardWidth());

    auto posX = Math::clamp<float>(positionX, 0.f, width);
    float rot = posX / width * 90.f;
    std::vector<float> volumes(INPUT_CHANNELS * OUTPUT_CHANNELS);
    volumes[0] = volumes[1] = Math::cos(rot);
    volumes[2] = volumes[3] = Math::sin(rot);

    selectOutput(volumes.data());
}

void SoundPan::panCenter() {
    const std::vector<float> volumes(INPUT_CHANNELS * OUTPUT_CHANNELS, CENTER_VOLUME);

    selectOutput(volumes.data());
}

void SoundPan::selectOutput(const float volumes[]) {
    const auto& outputVoices = mVoice.getOutputVoices();
    const auto descSize = outputVoices.size();
    //サイズが0ならマスターボイスへ
    if (descSize == 0) {
        setOutputMatrix(nullptr, volumes);
    } else {
        for (size_t i = 0; i < descSize; i++) {
            const auto& desc = outputVoices.getDesc(i);
            setOutputMatrix(desc.pOutputVoice, volumes);
        }
    }
}

void SoundPan::setOutputMatrix(IXAudio2Voice* outputVoice, const float volumes[]) {
    auto res = mVoice.getXAudio2Voice()->SetOutputMatrix(outputVoice, INPUT_CHANNELS, OUTPUT_CHANNELS, volumes);

#ifdef _DEBUG
    if (FAILED(res)) {
        Debug::logError("Failed pan.");
    }
#endif // _DEBUG
}
