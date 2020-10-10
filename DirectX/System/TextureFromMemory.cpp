#include "TextureFromMemory.h"
#include "../DirectX/DirectXInclude.h"

TextureFromMemory::TextureFromMemory(unsigned width, unsigned height) :
    Texture() {
    std::vector<unsigned char> data(width * height * 4, 255);
    createTextureFromMemory(data.data(), width, height);
}

TextureFromMemory::TextureFromMemory(unsigned char* data, unsigned width, unsigned height) :
    Texture() {
    createTextureFromMemory(data, width, height);
}

TextureFromMemory::~TextureFromMemory() = default;

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
