#include "Shader.h"
#include "../DebugLayer/Debug.h"
#include "../DirectX/DirectXInclude.h"

Shader::Shader(const std::string& fileName) :
    mCompileShader(nullptr),
    mVertexShader(nullptr),
    mPixelShader(nullptr),
    mVertexLayout(nullptr) {

    createVertexShader(fileName);
    createPixelShader(fileName);
}

Shader::~Shader() = default;

bool Shader::map(MappedSubResourceDesc* data, unsigned index, unsigned sub, D3D11_MAP type, unsigned flag) const {
    auto msr = toMappedSubResource(data);
    auto res = DirectX::instance().deviceContext()->Map(mConstantBuffers[index]->buffer(), sub, type, flag, &msr);

    data->data = msr.pData;
    data->rowPitch = msr.RowPitch;
    data->depthPitch = msr.DepthPitch;

    return (SUCCEEDED(res));
}

void Shader::unmap(unsigned index, unsigned sub) const {
    DirectX::instance().deviceContext()->Unmap(mConstantBuffers[index]->buffer(), sub);
}

void Shader::setVSShader(ID3D11ClassInstance* classInstances, unsigned numClassInstances) const {
    DirectX::instance().deviceContext()->VSSetShader(mVertexShader.Get(), &classInstances, numClassInstances);
}

void Shader::setPSShader(ID3D11ClassInstance* classInstances, unsigned numClassInstances) const {
    DirectX::instance().deviceContext()->PSSetShader(mPixelShader.Get(), &classInstances, numClassInstances);
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
    DirectX::instance().deviceContext()->VSSetConstantBuffers(index, numBuffers, &buf);
}

void Shader::setPSConstantBuffers(unsigned index, unsigned numBuffers) const {
    auto buf = mConstantBuffers[index]->buffer();
    DirectX::instance().deviceContext()->PSSetConstantBuffers(index, numBuffers, &buf);
}

void Shader::createInputLayout(const std::vector<InputElementDesc>& layout) {
    mVertexLayout = std::make_unique<InputElement>(layout, mCompileShader.Get());
}

void Shader::setInputLayout() const {
    DirectX::instance().deviceContext()->IASetInputLayout(mVertexLayout->layout());
}

void Shader::setShaderInfo(unsigned constantBufferIndex) const {
    setVSShader();
    setPSShader();
    setVSConstantBuffers(constantBufferIndex);
    setPSConstantBuffers(constantBufferIndex);
    setInputLayout();
}

void Shader::createVertexShader(const std::string& fileName) {
    //ブロブからバーテックスシェーダー作成
    if (FAILED(D3DX11CompileFromFileA(fileName.c_str(), nullptr, nullptr, "VS", "vs_5_0", 0, 0, nullptr, &mCompileShader, nullptr, nullptr))) {
        Debug::windowMessage(fileName + ": hlsl読み込み失敗");
        return;
    }
    if (FAILED(DirectX::instance().device()->CreateVertexShader(mCompileShader->GetBufferPointer(), mCompileShader->GetBufferSize(), nullptr, &mVertexShader))) {
        Debug::windowMessage(fileName + ": バーテックスシェーダー作成失敗");
        return;
    }
}

void Shader::createPixelShader(const std::string& fileName) {
    Microsoft::WRL::ComPtr<ID3DBlob> compiledShader;
    //ブロブからピクセルシェーダー作成
    if (FAILED(D3DX11CompileFromFileA(fileName.c_str(), nullptr, nullptr, "PS", "ps_5_0", 0, 0, nullptr, &compiledShader, nullptr, nullptr))) {
        Debug::windowMessage(fileName + ": hlsl読み込み失敗");
        return;
    }
    if (FAILED(DirectX::instance().device()->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &mPixelShader))) {
        Debug::windowMessage(fileName + ": ピクセルシェーダー作成失敗");
        return;
    }
}

D3D11_MAPPED_SUBRESOURCE Shader::toMappedSubResource(const MappedSubResourceDesc* desc) const {
    D3D11_MAPPED_SUBRESOURCE msr;
    msr.pData = desc->data;
    msr.RowPitch = desc->rowPitch;
    msr.DepthPitch = desc->depthPitch;

    return msr;
}
