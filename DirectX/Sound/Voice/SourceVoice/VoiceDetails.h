#pragma once

#include "../../Loader/WaveformData.h"
#include "../../../System/SystemInclude.h"

//サウンドデータ保存クラス
class VoiceDetails {
public:
    VoiceDetails(const WaveformData& data);
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
    const unsigned getInputChannels() const;

    /// <summary>
    /// サンプリングレートを返す
    /// </summary>
    /// <returns></returns>
    const unsigned getSampleRate() const;

    /// <summary>
    /// ビットレートを返す
    /// </summary>
    /// <returns></returns>
    const unsigned getBitRate() const;

private:
    VoiceDetails(const VoiceDetails&) = delete;
    VoiceDetails& operator=(const VoiceDetails&) = delete;

private:
    //ヘッダ等を含まない純粋な波形データ
    const BYTE* BUFFER;
    //波形データのサイズ
    unsigned mSize;
    //入力チャンネル数
    const unsigned INPUT_CHANNELS;
    //サンプリングレート
    const unsigned SAMPLES_PER_SEC;
    //ビットレート
    const unsigned BITS_PER_SAMPLE;
};
