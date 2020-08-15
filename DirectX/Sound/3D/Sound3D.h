#pragma once

#include "Listener/Sound3DListenerParam.h"
#include "../Loader/WaveFormat.h"
#include "../../Device/Flag.h"
#include <xaudio2.h>
#include <x3daudio.h>
#include <memory>
#include <vector>

class SourceVoice;
class MasteringVoice;
class Sound3DEmitter;
class DspSetter;

//3Dサウンドメインクラス
class Sound3D {
public:
    Sound3D(SourceVoice& sourceVoice, const MasteringVoice& masteringVoice, const WaveFormat& format);
    ~Sound3D();

    /// <summary>
    /// 3Dサウンドを計算する
    /// </summary>
    /// <param name="listenerParam">リスナー構造体</param>
    void calculate(const Sound3DListenerParam& listenerParam);

    /// <summary>
    /// エミッターを返す
    /// </summary>
    /// <returns></returns>
    Sound3DEmitter& getEmitter() const;

private:
    Sound3D(const Sound3D&) = delete;
    Sound3D& operator=(const Sound3D&) = delete;

    //フラグを設定し返す
    Flag setFlags();

private:
    //3Dサウンドのハンドル
    X3DAUDIO_HANDLE m3DInstance;
    //エミッター(音源)
    std::unique_ptr<Sound3DEmitter> mEmitter;
    //3D計算結果
    std::unique_ptr<DspSetter> mDspSetter;
    //初期化に成功しているか
    bool mInitialized;
};
