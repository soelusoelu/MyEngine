#include "TextureFromMemory.h"
#include "../DirectX/DirectXInclude.h"
#include <algorithm>

TextureFromMemory::TextureFromMemory(unsigned width, unsigned height) :
    Texture(),
    mPixelData(width * height * 4, 255) {
    createTextureFromMemory(mPixelData.data(), width, height);
}

TextureFromMemory::TextureFromMemory(const std::vector<unsigned char>& data, unsigned width, unsigned height) :
    Texture(),
    mPixelData(width * height * 4) {
    std::copy(data.begin(), data.end(), mPixelData.begin());
    createTextureFromMemory(mPixelData.data(), width, height);
}

TextureFromMemory::~TextureFromMemory() = default;

void TextureFromMemory::setPixel(unsigned width, unsigned height, const Vector3 & color) {
    //データがunsigned char型で色3つアルファ値1つ並んでるから
    auto p = height * mDesc.width + width;
    mPixelData[p] = 255 * color.x;
    mPixelData[p + 1] = 255 * color.y;
    mPixelData[p + 2] = 255 * color.z;
}

void TextureFromMemory::setPixel(unsigned width, unsigned height, unsigned char r, unsigned char g, unsigned char b) {
    //データがunsigned char型で色3つアルファ値1つ並んでるから
    auto p = height * mDesc.width + width;
    mPixelData[p] = r;
    mPixelData[p + 1] = g;
    mPixelData[p + 2] = b;
}

void TextureFromMemory::apply() {
    createTextureFromMemory(mPixelData.data(), mDesc.width, mDesc.height);
}

void TextureFromMemory::createTextureFromMemory(unsigned char* data, unsigned width, unsigned height) {
    mDesc.width = width;
    mDesc.height = height;

    Texture2DDesc tex2DDesc;
    tex2DDesc.width = width;
    tex2DDesc.height = height;

    SubResourceDesc sub;
    sub.data = data;
    sub.pitch = width * 4;

    auto tex2D = std::make_unique<Texture2D>(tex2DDesc, sub);

    mShaderResourceView = std::make_unique<ShaderResourceView>(*tex2D);
}
