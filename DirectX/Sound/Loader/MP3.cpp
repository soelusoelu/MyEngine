#include "MP3.h"
#include "FrameHeader.h"
#include "SoundFormatConverter.h"
#include "../../DebugLayer/Debug.h"
#include <cassert>
#include <filesystem>
#include <string>

MP3::MP3() :
    mIfs(),
    mFrameHeader(std::make_unique<FrameHeader>()),
    mFormatConverter(std::make_unique<SoundFormatConverter>()),
    mDataSize(0),
    mDataOffset(0) {
}

MP3::~MP3() = default;

bool MP3::loadFromFile(WAVEFORMATEX* format, const std::string& fileName) {
    mIfs.open(fileName, std::ios::in | std::ios::binary);
    if (!mIfs.is_open()) {
        Debug::logError("Failed " + fileName + " opened.");
    }

    //ファイル全体のサイズ
    auto fileSize = std::filesystem::file_size(fileName);

    BYTE header[HEADER_SIZE];
    mIfs.read(reinterpret_cast<char*>(header), HEADER_SIZE);
    //ID3v2である
    if (memcmp(header, "ID3", 3) == 0) {
        unsigned tagSize = ((header[6] << 21) | (header[7] << 14) | (header[8] << 7) | (header[9]));
        //データ部分へのオフセット値決定
        mDataOffset = tagSize + 10;
        //データ部分のサイズ決定
        mDataSize = fileSize - mDataOffset;
    } else {
        //ここに入り次第処理書く
        assert(false);
    }

    //フレームヘッダ
    BYTE frameHeader[FrameHeader::FRAME_HEADER_SIZE];
    MPEGLAYER3WAVEFORMAT mp3wf = { 0 };
    while (!mIfs.eof()) {
        mIfs.read(reinterpret_cast<char*>(frameHeader), FrameHeader::FRAME_HEADER_SIZE);
        if (mFrameHeader->isFrameHeader(frameHeader)) {
            if (mFrameHeader->parseFrameHeader(header)) {
                break;
            }
        }
    }

    //フレームヘッダで解析したデータをMPEGLAYER3WAVEFORMATに詰め込む
    mFrameHeader->setDataToMpegLayer3WaveFormat(&mp3wf);
    format->wFormatTag = WAVE_FORMAT_PCM;
    format->cbSize = sizeof(WAVEFORMATEX);

    //mp3からpcmへデータ変換する
    mFormatConverter->mp3ToPCM(&format, mp3wf);

    //データサイズをデコード
    mFormatConverter->decodeSize(mDataSize, &mDataSize);

    //ファイルポインタをデータの先頭に戻しておく
    seekBegin();

    return true;
}

long MP3::read(BYTE** buffer, unsigned size) {
    unsigned decodeSize = 0;
    mFormatConverter->decodeSize(size, &decodeSize, false);
    mIfs.read(reinterpret_cast<char*>(*buffer), decodeSize);
    auto count = mIfs.gcount();
    auto readSize = mFormatConverter->convert(buffer, count);
    return readSize;
}

long MP3::seek(long offset, Seek seek) {
    mIfs.clear();
    auto s = static_cast<std::ios_base::_Seekdir>(seek);
    if (mIfs.seekg(offset, s)) {
        return offset;
    } else {
        return -1;
    }
}

void MP3::seekBegin() {
    seek(mDataOffset, Seek::BEGIN);
}

unsigned MP3::size() const {
    return mDataSize;
}
