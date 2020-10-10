#pragma once

#include "Texture.h"
#include <vector>

class TextureFromMemory : public Texture {
public:
    TextureFromMemory(unsigned width, unsigned height);
    TextureFromMemory(unsigned char* data, unsigned width, unsigned height);
    ~TextureFromMemory();

private:
    //メモリデータからテクスチャを作成する
    void createTextureFromMemory(unsigned char* data, unsigned width, unsigned height);

private:
    std::vector<unsigned char> mPixelData;
};
