#pragma once

#include "ISoundLoader.h"
#include <mmreg.h>
#include <fstream>
#include <memory>

class FrameHeader;
class SoundFormatConverter;

class MP3 : public ISoundLoader {
public:
    MP3();
    ~MP3();
    virtual bool loadFromFile(WAVEFORMATEX* format, const std::string& fileName) override;
    virtual long read(BYTE** buffer, unsigned size) override;
    virtual long seek(long offset, Seek seek = Seek::CURRENT) override;
    virtual void seekBegin() override;
    virtual unsigned size() const override;

private:
    MP3(const MP3&) = delete;
    MP3& operator=(const MP3&) = delete;

private:
    std::ifstream mIfs;
    std::unique_ptr<FrameHeader> mFrameHeader;
    std::unique_ptr<SoundFormatConverter> mFormatConverter;
    //データ部分のサイズ
    unsigned mDataSize;
    //ファイルの先頭からデータへのオフセット値
    unsigned mDataOffset;

    //MP3ヘッダのサイズ
    static constexpr unsigned HEADER_SIZE = 10;
};
