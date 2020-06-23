﻿#include "Shader.h"
#include "GlobalFunction.h"
#include "../DebugLayer/Debug.h"
#include "../DirectX/Buffer.h"
#include "../DirectX/BufferDesc.h"
#include "../DirectX/DirectX.h"
#include "../DirectX/InputElement.h"
#include "../DirectX/InputElementDesc.h"
#include "../DirectX/Usage.h"
#include "../Utility/Directory.h"

Shader::Shader(const std::string& fileName) :
    mCompileShader(nullptr),
    mVertexShader(nullptr),
    mPixelShader(nullptr),
    mVertexLayout(nullptr) {

    createVertexShader(fileName);
    createPixelShader(fileName);
}

Shader::~Shader() {
    safeRelease<ID3D10Blob>(mCompileShader);
    safeRelease<ID3D11VertexShader>(mVertexShader);
    safeRelease<ID3D11PixelShader>(mPixelShader);
}

bool Shader::map(MappedSubResourceDesc* data, unsigned index, unsigned sub, D3D11_MAP type, unsigned flag) const {
    auto msr = toMappedSubResource(data);
    auto res = Singleton<DirectX>::instance().deviceContext()->Map(mConstantBuffers[index]->buffer(), sub, type, flag, &msr);

    data->data = msr.pData;
    data->rowPitch = msr.RowPitch;
    data->depthPitch = msr.DepthPitch;

    return (SUCCEEDED(res));
}

void Shader::unmap(unsigned index, unsigned sub) const {
    Singleton<DirectX>::instance().deviceContext()->Unmap(mConstantBuffers[index]->buffer(), sub);
}

void Shader::setVSShader(ID3D11ClassInstance* classInstances, unsigned numClassInstances) const {
    Singleton<DirectX>::instance().deviceContext()->VSSetShader(mVertexShader, &classInstances, numClassInstances);
}

void Shader::setPSShader(ID3D11ClassInstance* classInstances, unsigned numClassInstances) const {
    Singleton<DirectX>::instance().deviceContext()->PSSetShader(mPixelShader, &classInstances, numClassInstances);
}

void Shader::createConstantBuffer(unsigned bufferSize, unsigned index) {
    auto num = mConstantBuffers.size();
    if (index >= num) {
        mConstantBuffers.resize(num + 1);
    }

    BufferDesc cb;
    cb.size = bufferSize;
    cb.usage = Usage::USAGE_DYNAMIC;
    cb.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_CONSTANT_BUFFER);
    cb.cpuAccessFlags = static_cast<unsigned>(BufferCPUAccessFlag::CPU_ACCESS_WRITE);

    mConstantBuffers[index] = std::make_unique<Buffer>(cb);
}

void Shader::setVSConstantBuffers(unsigned index, unsigned numBuffers) const {
    auto buf = mConstantBuffers[index]->buffer();
    Singleton<DirectX>::instance().deviceContext()->VSSetConstantBuffers(index, numBuffers, &buf);
}

void Shader::setPSConstantBuffers(unsigned index, unsigned numBuffers) const {
    auto buf = mConstantBuffers[index]->buffer();
    Singleton<DirectX>::instance().deviceContext()->PSSetConstantBuffers(index, numBuffers, &buf);
}

void Shader::createInputLayout(const InputElementDesc layout[], unsigned numElements) {
    mVertexLayout = std::make_unique<InputElement>(layout, numElements, mCompileShader);
}

void Shader::setInputLayout() const {
    Singleton<DirectX>::instance().deviceContext()->IASetInputLayout(mVertexLayout->layout());
}

void Shader::createVertexShader(const std::string& fileName) {
    Singleton<Directory>::instance().setShaderDirectory();
    //ブロブからバーテックスシェーダー作成
    if (FAILED(D3DX11CompileFromFileA(fileName.c_str(), nullptr, nullptr, "VS", "vs_5_0", 0, 0, nullptr, &mCompileShader, nullptr, nullptr))) {
        Debug::windowMessage(fileName + ": hlsl読み込み失敗");
        return;
    }
    if (FAILED(Singleton<DirectX>::instance().device()->CreateVertexShader(mCompileShader->GetBufferPointer(), mCompileShader->GetBufferSize(), nullptr, &mVertexShader))) {
        safeRelease<ID3D10Blob>(mCompileShader);
        Debug::windowMessage(fileName + ": バーテックスシェーダー作成失敗");
        return;
    }
}

void Shader::createPixelShader(const std::string& fileName) {
    ID3D10Blob* compiledShader;
    Singleton<Directory>::instance().setShaderDirectory();
    //ブロブからピクセルシェーダー作成
    if (FAILED(D3DX11CompileFromFileA(fileName.c_str(), nullptr, nullptr, "PS", "ps_5_0", 0, 0, nullptr, &compiledShader, nullptr, nullptr))) {
        Debug::windowMessage(fileName + ": hlsl読み込み失敗");
        return;
    }
    if (FAILED(Singleton<DirectX>::instance().device()->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &mPixelShader))) {
        safeRelease<ID3D10Blob>(compiledShader);
        Debug::windowMessage(fileName + ": ピクセルシェーダー作成失敗");
        return;
    }
    safeRelease<ID3D10Blob>(compiledShader);
}

D3D11_MAPPED_SUBRESOURCE Shader::toMappedSubResource(const MappedSubResourceDesc* desc) const {
    D3D11_MAPPED_SUBRESOURCE msr;
    msr.pData = desc->data;
    msr.RowPitch = desc->rowPitch;
    msr.DepthPitch = desc->depthPitch;

    return msr;
}
