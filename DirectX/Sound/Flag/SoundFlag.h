#pragma once

#include <xaudio2.h>
#include <x3daudio.h>

//サウンド関連フラグ
namespace SoundFlags {
constexpr unsigned NOPITCH = XAUDIO2_VOICE_NOPITCH;
constexpr unsigned NOSRC = XAUDIO2_VOICE_NOSRC;
constexpr unsigned USE_FILTER = XAUDIO2_VOICE_USEFILTER;
}

//3Dサウンド使われるフラグ
namespace Sound3DFlags {
//行列係数テーブルの計算を有効化
constexpr unsigned CALCULATE_MATRIX = X3DAUDIO_CALCULATE_MATRIX;
//遅延時間配列計算を有効化(ステレオのみ)
constexpr unsigned CALCULATE_DELAY = X3DAUDIO_CALCULATE_DELAY;
//ローパスフィルタ直接パス係数計算を有効化
constexpr unsigned CALCULATE_LPF_DIRECT = X3DAUDIO_CALCULATE_LPF_DIRECT;
//ローパスフィルタリバーブパス係数の計算を有効化
constexpr unsigned CALCULATE_LPF_REVERB = X3DAUDIO_CALCULATE_LPF_REVERB;
//リバーブセンドレベルの計算を有効化
constexpr unsigned CALCULATE_REVERB = X3DAUDIO_CALCULATE_REVERB;
//ドップラーシフト係数の計算を有効化
constexpr unsigned CALCULATE_DOPPLER = X3DAUDIO_CALCULATE_DOPPLER;
//エミッターからリスナーへの内角計算を有効化
constexpr unsigned CALCULATE_EMITTER_ANGLE = X3DAUDIO_CALCULATE_EMITTER_ANGLE;

//センターチャンネルを無音で埋める CALCULATE_MATRIXを設定した場合のみ有効
constexpr unsigned CALCULATE_ZEROCENTER = X3DAUDIO_CALCULATE_ZEROCENTER;
//すべてのソースチャンネルの等しいミックスを低周波効果(LFE)の宛先チャンネルに適用します CALCULATE_MATRIXを設定した場合のみ有効
constexpr unsigned CALCULATE_REDIRECT_TO_LFE = X3DAUDIO_CALCULATE_REDIRECT_TO_LFE;
}
