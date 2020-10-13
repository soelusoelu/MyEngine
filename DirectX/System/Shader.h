#pragma once

#include "SystemInclude.h"
#include "../DirectX/InputElementDesc.h"
#include <memory>
#include <string>
#include <vector>

class Buffer;
class InputElement;

class Shader {
public:
    Shader(const std::string& fileName);
    ~Shader();

    //シェーダーにデータを転送する
    void transferData(const void* data, unsigned size, unsigned index = 0) const;

    //コンスタントバッファの作成
    void createConstantBuffer(unsigned bufferSize, unsigned index = 0);
    //インプットレイアウトの生成
    void createInputLayout(const std::vector<InputElementDesc>& layout);

    //自身をシェーダーとして登録
    void setVSShader(ID3D11ClassInstance* classInstances = nullptr, unsigned numClassInstances = 0) const;
    void setPSShader(ID3D11ClassInstance* classInstances = nullptr, unsigned numClassInstances = 0) const;
    //使用するコンスタントバッファを登録
    void setVSConstantBuffers(unsigned index = 0, unsigned numBuffers = 1) const;
    void setPSConstantBuffers(unsigned index = 0, unsigned numBuffers = 1) const;
    //自身を登録
    void setInputLayout() const;
    //描画に必要な要素をすべて登録する
    void setShaderInfo(unsigned constantBufferIndex = 0) const;

private:
    //シェーダの生成
    void createVertexShader(const std::string& fileName);
    void createPixelShader(const std::string& fileName);
    //シェーダーに値を渡すための開始・終了処理
    void map(D3D11_MAPPED_SUBRESOURCE* mapRes, unsigned index = 0, unsigned sub = 0, D3D11_MAP type = D3D11_MAP_WRITE_DISCARD, unsigned flag = 0) const;
    void unmap(unsigned index = 0, unsigned sub = 0) const;

private:
    Microsoft::WRL::ComPtr<ID3DBlob> mCompileShader;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
    std::vector<std::unique_ptr<Buffer>> mConstantBuffers;
    std::unique_ptr<InputElement> mVertexLayout;
};
