#include "VoiceDetails.h"
#include <xaudio2.h>

VoiceDetails::VoiceDetails(const WaveformData& data) :
    BUFFER(data.buffer),
    mSize(data.size),
    INPUT_CHANNELS(data.format->nChannels),
    SAMPLES_PER_SEC(data.format->nSamplesPerSec),
    BITS_PER_SAMPLE(data.format->wBitsPerSample) {
}

VoiceDetails::~VoiceDetails() = default;

const BYTE* VoiceDetails::buffer() const {
    return BUFFER;
}

unsigned VoiceDetails::size() const {
    return mSize;
}

const unsigned VoiceDetails::getInputChannels() const {
    return INPUT_CHANNELS;
}

const unsigned VoiceDetails::getSampleRate() const {
    return SAMPLES_PER_SEC;
}

const unsigned VoiceDetails::getBitRate() const {
    return BITS_PER_SAMPLE;
}
