﻿#include "Texture.h"
#include "../GlobalFunction.h"
#include "../Shader/Shader.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../Engine/DebugManager/DebugUtility/Debug.h"

Texture::Texture() :
    mShaderResourceView(nullptr),
    mSampler(nullptr),
    mTextureSize() {
    if (!vertexBuffer || !indexBuffer) {
        //バーテックスバッファー作成
        createVertexBuffer();
        //インデックスバッファの作成
        createIndexBuffer();
    }
    //テクスチャー用サンプラー作成
    createSampler();
}

Texture::Texture(const std::shared_ptr<ShaderResourceView>& view, const Vector2& textureSize)
    : mShaderResourceView(view)
    , mSampler(nullptr)
    , mTextureSize(textureSize)
{
    if (!vertexBuffer || !indexBuffer) {
        //バーテックスバッファー作成
        createVertexBuffer();
        //インデックスバッファの作成
        createIndexBuffer();
    }
    //テクスチャー用サンプラー作成
    createSampler();
}

Texture::~Texture() = default;

const Vector2& Texture::getTextureSize() const {
    return mTextureSize;
}

const ShaderResourceView& Texture::getShaderResourceView() const {
    return *mShaderResourceView;
}

const Sampler& Texture::getSampler() const {
    return *mSampler;
}

void Texture::finalize() {
    safeDelete(vertexBuffer);
    safeDelete(vertexBuffer3D);
    safeDelete(indexBuffer);
}

void Texture::createVertexBuffer() {
    static const TextureVertex vertices[] = {
        Vector3(0.f, 0.f, 0.f), Vector2(0.f, 0.f), //左上
        Vector3(1.f, 0.f, 0.f), Vector2(1.f, 0.f), //右上
        Vector3(0.f, 1.f, 0.f), Vector2(0.f, 1.f), //左下
        Vector3(1.f, 1.f, 0.f), Vector2(1.f, 1.f), //右下
    };

    BufferDesc bd;
    bd.oneSize = sizeof(TextureVertex);
    bd.size = bd.oneSize * 4; //頂点が4つだから
    bd.usage = Usage::IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::VERTEX);

    SubResourceDesc sub;
    sub.data = vertices;

    vertexBuffer = new VertexBuffer(bd, &sub);

    static const TextureVertex vertices3D[] = {
        Vector3(-0.5f, 0.5f, 0.f), Vector2(0.f, 0.f), //左上
        Vector3(0.5f, 0.5f, 0.f), Vector2(1.f, 0.f), //右上
        Vector3(-0.5f, -0.5f, 0.f), Vector2(0.f, 1.f), //左下
        Vector3(0.5f, -0.5f, 0.f), Vector2(1.f, 1.f), //右下
    };

    sub.data = vertices3D;

    vertexBuffer3D = new VertexBuffer(bd, &sub);
}

void Texture::createIndexBuffer() {
    static constexpr unsigned short indices[] = {
        0, 1, 2,
        1, 3, 2
    };
    BufferDesc bd;
    bd.size = sizeof(indices);
    bd.usage = Usage::IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::INDEX);

    SubResourceDesc sub;
    sub.data = indices;
    indexBuffer = new IndexBuffer(bd, sub);
}

void Texture::createSampler() {
    SamplerDesc sd;
    mSampler = std::make_unique<Sampler>(sd);
}
