#pragma once

//アウトプット用(MasteringVoice用)サウンドデータ保存用クラス
struct OutputVoiceDetails {
    unsigned creationFlags;
    unsigned activeFlags;
    unsigned outputChannels;
    unsigned outputSampleRate;

    OutputVoiceDetails() :
        creationFlags(0),
        activeFlags(0),
        outputChannels(0),
        outputSampleRate(0) {
    }
};