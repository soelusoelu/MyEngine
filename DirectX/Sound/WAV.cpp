#include "WAV.h"
#include "../System/World.h"
#include "../Utility/Directory.h"

#pragma comment(lib, "winmm.lib")

WAV::WAV() = default;
WAV::~WAV() = default;

void WAV::loadFromFile(const std::string& fileName) {
    World::instance().directory().setSoundDirectory();

    HMMIO hMmio = NULL; //WindowsマルチメディアAPIのハンドル(WindowsマルチメディアAPIはWAVファイル関係の操作用のAPI)
    MMCKINFO ckInfo; //チャンク情報
    MMCKINFO riffckInfo; //最上部チャンク(RIFFチャンク)保存用

    //WAVファイル内のヘッダー情報(音データ以外)の確認と読み込み
    char* fn = const_cast<char*>(fileName.c_str()); //他にやり方思い浮かばない
    hMmio = mmioOpenA(fn, NULL, MMIO_ALLOCBUF | MMIO_READ);

    //ファイルポインタをRIFFチャンクの先頭にセットする
    mmioDescend(hMmio, &riffckInfo, NULL, 0);

    //ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
    ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
    mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);

    //フォーマットを読み込む
    PCMWAVEFORMAT pcmWaveForm;
    mmioRead(hMmio, reinterpret_cast<HPSTR>(&pcmWaveForm), sizeof(pcmWaveForm));
    mFormat = reinterpret_cast<WAVEFORMATEX*>(new CHAR[sizeof(WAVEFORMATEX)]);
    memcpy(mFormat, &pcmWaveForm, sizeof(pcmWaveForm));
    mFormat->cbSize = 0;
    mmioAscend(hMmio, &ckInfo, 0);

    //WAVファイル内の音データの読み込み
    ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
    mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK); //データチャンクにセット
    mSize = ckInfo.cksize;
    mBuffer = new BYTE[mSize];
    DWORD offset = ckInfo.dwDataOffset;
    mmioRead(hMmio, reinterpret_cast<HPSTR>(mBuffer), mSize);
}
