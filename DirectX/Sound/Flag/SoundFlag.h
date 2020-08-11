#pragma once

#include <xaudio2.h>

//サウンド関連フラグ
enum class SoundFlag {
    NOPITCH = XAUDIO2_VOICE_NOPITCH,
    NOSRC = XAUDIO2_VOICE_NOSRC,
    USE_FILTER = XAUDIO2_VOICE_USEFILTER
};
