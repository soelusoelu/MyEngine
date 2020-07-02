#pragma once

#include "../System/SystemInclude.h"

class SoundLoader;

//サウンドデータ保存クラス
class VoiceDetails {
public:
    VoiceDetails(const SoundLoader& loader);
    ~VoiceDetails();

    /// <summary>
    /// ヘッダ等を含まない純粋な波形データを返す
    /// </summary>
    /// <returns></returns>
    const BYTE* buffer() const;

    /// <summary>
    /// 波形データのサイズを返す
    /// </summary>
    /// <returns></returns>
    unsigned size() const;

    /// <summary>
    /// 入力チャンネル数を返す
    /// </summary>
    /// <returns></returns>
    unsigned getInputChannels() const;

    /// <summary>
    /// サンプリングレートを返す
    /// </summary>
    /// <returns></returns>
    unsigned getSampleRate() const;

    /// <summary>
    /// ビットレートを返す
    /// </summary>
    /// <returns></returns>
    unsigned getBitRate() const;

private:
    //ヘッダ等を含まない純粋な波形データ
    const BYTE* mBuffer;
    //波形データのサイズ
    unsigned mSize;
    //入力チャンネル数
    unsigned mInputChannels;
    //サンプリングレート
    unsigned mSamplesPerSec;
    //ビットレート
    unsigned mBitsPerSample;
};
