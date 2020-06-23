﻿#pragma once

#include "SystemInclude.h"
#include "../DirectX/InputElementDesc.h"
#include "../DirectX/SubResourceDesc.h"
#include <memory>
#include <string>
#include <vector>

class Buffer;
class InputElement;

class Shader {
public:
    Shader(const std::string& fileName);
    ~Shader();
    //シェーダーとのやり取り
    bool map(MappedSubResourceDesc* data, unsigned index = 0, unsigned sub = 0, D3D11_MAP type = D3D11_MAP_WRITE_DISCARD, unsigned flag = 0) const;
    void unmap(unsigned index = 0, unsigned sub = 0) const;
    //自身をシェーダーとして登録
    void setVSShader(ID3D11ClassInstance* classInstances = nullptr, unsigned numClassInstances = 0) const;
    void setPSShader(ID3D11ClassInstance* classInstances = nullptr, unsigned numClassInstances = 0) const;
    //コンスタントバッファの作成
    void createConstantBuffer(unsigned bufferSize, unsigned index = 0);
    //使用するコンスタントバッファを登録
    void setVSConstantBuffers(unsigned index = 0, unsigned numBuffers = 1) const;
    void setPSConstantBuffers(unsigned index = 0, unsigned numBuffers = 1) const;
    //インプットレイアウトの生成
    void createInputLayout(const InputElementDesc layout[], unsigned numElements);
    //自身を登録
    void setInputLayout() const;

private:
    //シェーダの生成
    void createVertexShader(const std::string& fileName);
    void createPixelShader(const std::string& fileName);
    D3D11_MAPPED_SUBRESOURCE toMappedSubResource(const MappedSubResourceDesc* desc) const;

private:
    ID3D10Blob* mCompileShader;
    ID3D11VertexShader* mVertexShader;
    ID3D11PixelShader* mPixelShader;
    std::vector<std::unique_ptr<Buffer>> mConstantBuffers;
    std::unique_ptr<InputElement> mVertexLayout;
};
