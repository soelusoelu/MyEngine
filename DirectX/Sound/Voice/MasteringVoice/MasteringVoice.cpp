#include "MasteringVoice.h"

MasteringVoice::MasteringVoice(IXAudio2MasteringVoice* XAudio2MasteringVoice) :
    mXAudio2MasteringVoice(XAudio2MasteringVoice),
    mDetails()
{
    XAUDIO2_VOICE_DETAILS details;
    mXAudio2MasteringVoice->GetVoiceDetails(&details);
    toOutputVoiceDetails(details);
}

MasteringVoice::~MasteringVoice() {
    mXAudio2MasteringVoice->DestroyVoice();
    mXAudio2MasteringVoice = nullptr;
}

const OutputVoiceDetails& MasteringVoice::getDetails() const {
    return mDetails;
}

void MasteringVoice::toOutputVoiceDetails(const XAUDIO2_VOICE_DETAILS& details) {
    mDetails.activeFlags = details.ActiveFlags;
    mDetails.creationFlags = details.CreationFlags;
    mDetails.outputChannels = details.InputChannels;
    mDetails.outputSampleRate = details.InputSampleRate;
}
