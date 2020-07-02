#include "SoundLoader.h"
#include "../System/GlobalFunction.h"

SoundLoader::SoundLoader() :
    mBuffer(nullptr),
    mSize(0),
    mFormat(nullptr),
    mChannels() {
}

SoundLoader::~SoundLoader() {
    safeDelete(mBuffer);
    safeDelete(mFormat);
}

const BYTE* SoundLoader::buffer() const {
    return mBuffer;
}

unsigned SoundLoader::size() const {
    return mSize;
}

WAVEFORMATEX* SoundLoader::format() const {
    return mFormat;
}
