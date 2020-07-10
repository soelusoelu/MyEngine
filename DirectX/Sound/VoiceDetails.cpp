#include "VoiceDetails.h"
#include "SoundLoader.h"
#include <xaudio2.h>

VoiceDetails::VoiceDetails(const SoundLoader& loader) :
    mBuffer(loader.buffer()),
    mSize(loader.size()),
    INPUT_CHANNELS(loader.format()->nChannels),
    SAMPLES_PER_SEC(loader.format()->nSamplesPerSec),
    BITS_PER_SAMPLE(loader.format()->wBitsPerSample) {
}

VoiceDetails::~VoiceDetails() = default;

const BYTE* VoiceDetails::buffer() const {
    return mBuffer;
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
