#pragma once

#include <xaudio2.h>
#include <string>

//サウンド読み込み用基底クラス
class SoundLoader {
public:
    SoundLoader();
    virtual ~SoundLoader();
    virtual void loadFromFile(const std::string& fileName) = 0;

    //波形データの取得
    const BYTE* buffer() const;
    //波形データのサイズの取得
    unsigned size() const;
    //フォーマットの取得
    WAVEFORMATEX* format() const;

protected:
    BYTE* mBuffer; //波形データ(フォーマット等を含まない、純粋に波形データのみ)
    unsigned mSize; //波形データのサイズ
    WAVEFORMATEX* mFormat;
};
