#pragma once

#include "../Loader/WaveFormat.h"
#include <xaudio2.h>
#include <x3daudio.h>
#include <memory>
#include <vector>

class SourceVoice;
class MasteringVoice;
class Sound3DListener;
class Sound3DEmitter;
class DspSetter;

//3Dサウンドメインクラス
class Sound3D {
public:
    Sound3D(SourceVoice& sourceVoice, const MasteringVoice& masteringVoice, const WaveFormat& format);
    ~Sound3D();

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// リスナーを返す
    /// </summary>
    /// <returns></returns>
    Sound3DListener& getListener() const;

    /// <summary>
    /// エミッターを返す
    /// </summary>
    /// <returns></returns>
    Sound3DEmitter& getEmitter() const;

private:
    Sound3D(const Sound3D&) = delete;
    Sound3D& operator=(const Sound3D&) = delete;

    //3Dサウンドを計算する
    void calculate();

private:
    //3Dサウンドのハンドル
    X3DAUDIO_HANDLE m3DInstance;
    //リスナー(聞き手)
    std::unique_ptr<Sound3DListener> mListener;
    //エミッター(音源)
    std::unique_ptr<Sound3DEmitter> mEmitter;
    //3D計算結果
    std::unique_ptr<DspSetter> mDspSetter;
    //初期化に成功しているか
    bool mInitialized;
    //フラグ
    unsigned mFlags;
};
