#include "VoiceDetails.h"
#include "SoundLoader.h"
#include <xaudio2.h>

VoiceDetails::VoiceDetails(const SoundLoader& loader) :
    mBuffer(loader.buffer()),
    mSize(loader.size()),
    mInputChannels(loader.format()->nChannels),
    mSamplesPerSec(loader.format()->nSamplesPerSec),
    mBitsPerSample(loader.format()->wBitsPerSample) {
}

VoiceDetails::~VoiceDetails() = default;

const BYTE* VoiceDetails::buffer() const {
    return mBuffer;
}

unsigned VoiceDetails::size() const {
    return mSize;
}

unsigned VoiceDetails::getInputChannels() const {
    return mInputChannels;
}

unsigned VoiceDetails::getSampleRate() const {
    return mSamplesPerSec;
}

unsigned VoiceDetails::getBitRate() const {
    return mBitsPerSample;
}
