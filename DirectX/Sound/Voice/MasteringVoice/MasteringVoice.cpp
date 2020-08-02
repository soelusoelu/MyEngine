#include "MasteringVoice.h"

MasteringVoice::MasteringVoice(IXAudio2MasteringVoice* XAudio2MasteringVoice) :
    mXAudio2MasteringVoice(XAudio2MasteringVoice),
    mDetails()
{
    XAUDIO2_VOICE_DETAILS details;
    mXAudio2MasteringVoice->GetVoiceDetails(&details);
    mDetails.channels = details.InputChannels;
    mDetails.sampleRate = details.InputSampleRate;
}

MasteringVoice::~MasteringVoice() {
    mXAudio2MasteringVoice->DestroyVoice();
    mXAudio2MasteringVoice = nullptr;
}

const VoiceDetails& MasteringVoice::getDetails() const {
    return mDetails;
}
