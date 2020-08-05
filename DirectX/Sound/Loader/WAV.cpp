#include "WAV.h"
#include "../../DebugLayer/Debug.h"

WAV::WAV() :
    mHMmio(nullptr),
    mChunkInfo(),
    mRiffChunkInfo() {
}

WAV::~WAV() = default;

std::shared_ptr<WaveformData> WAV::loadFromFile(const std::string& fileName) {
    //WAVファイル内のヘッダー情報(音データ以外)の確認と読み込み
    open(fileName);

    //ファイルポインタをRIFFチャンクの先頭にセットする
    if (!descend(&mRiffChunkInfo, nullptr, FindFlag::NONE)) {
        return nullptr;
    }

    //一応Wavファイルか確認
    if (!isWavFile(mRiffChunkInfo)) {
        return nullptr;
    }

    //ファイルポインタをfmtチャンクにセットする
    setChunkID(CHUNK_FORMAT);
    if (!descend(&mChunkInfo, &mRiffChunkInfo, FindFlag::CHUNK)) {
        return nullptr;
    }

    //PCMフォーマットを読み込む
    PCMWAVEFORMAT pcmWaveFmt;
    if (read(&pcmWaveFmt, sizeof(pcmWaveFmt)) != sizeof(pcmWaveFmt)) {
        Debug::logWarning("Failed to read wav format.");
        return nullptr;
    }

    auto out = std::make_shared<WaveformData>();
    //フォーマット作成
    createWaveFormat(&out->format, pcmWaveFmt);
    if (!ascend(&mChunkInfo)) {
        return nullptr;
    }

    //WAVファイル内の音データの読み込み
    setChunkID(CHUNK_DATA);
    if (!descend(&mChunkInfo, &mRiffChunkInfo, FindFlag::CHUNK)) {
        return nullptr;
    }

    out->size = mChunkInfo.cksize;
    out->buffer = new BYTE[out->size];

    if (read(out->buffer, out->size) != out->size) {
        Debug::logWarning("Failed to read wav format.");
        return nullptr;
    }

    return out;
}

void WAV::open(const std::string& fileName) {
    char* fn = const_cast<char*>(fileName.c_str()); //他にやり方思い浮かばない
    mHMmio = mmioOpenA(fn, nullptr, MMIO_ALLOCBUF | MMIO_READ);
}

long WAV::read(void* out, long size) const {
    return mmioRead(mHMmio, reinterpret_cast<char*>(out), size);
}

bool WAV::descend(MMCKINFO* out, const MMCKINFO* parent, FindFlag flag) const {
    auto res = mmioDescend(mHMmio, out, parent, static_cast<unsigned>(flag));
    if (res != MMSYSERR_NOERROR) {
        Debug::logError("Failed descend.");
        return false;
    }
    return true;
}

bool WAV::ascend(MMCKINFO* out) {
    auto res = mmioAscend(mHMmio, out, 0);
    if (res != MMSYSERR_NOERROR) {
        Debug::logError("Failed ascend.");
        return false;
    }
    return true;
}

void WAV::setChunkID(const char* ch) {
    mChunkInfo.ckid = getFourCC(ch);
}

FOURCC WAV::getFourCC(const char* ch) const {
    return mmioFOURCC(ch[0], ch[1], ch[2], ch[3]);
}

bool WAV::isWavFile(const MMCKINFO& riffChunk) const {
    if (riffChunk.ckid != getFourCC(CHUNK_RIFF)) {
        Debug::logWarning("Chunk ID does not match \"riff\".");
        return false;
    }
    if (riffChunk.fccType != getFourCC(CHUNK_WAVE)) {
        Debug::logWarning("FourCC Type does not match \"wave\".");
        return false;
    }
    return true;
}

void WAV::createWaveFormat(WAVEFORMATEX* dst, const PCMWAVEFORMAT& src) {
    dst->wFormatTag = src.wf.wFormatTag;
    dst->nChannels = src.wf.nChannels;
    dst->nSamplesPerSec = src.wf.nSamplesPerSec;
    dst->nAvgBytesPerSec = src.wf.nAvgBytesPerSec;
    dst->nBlockAlign = src.wf.nBlockAlign;
    dst->wBitsPerSample = src.wBitsPerSample;
    dst->cbSize = 0;
}
