﻿#include "GBuffer.h"
#include "Shader.h"
#include "Window.h"
#include "../Component/Camera/Camera.h"
#include "../Component/Light/DirectionalLight.h"
#include "../Device/AssetsManager.h"
#include "../DirectX/BlendDesc.h"
#include "../DirectX/BlendState.h"
#include "../DirectX/Buffer.h"
#include "../DirectX/BufferDesc.h"
#include "../DirectX/DepthStencilState.h"
#include "../DirectX/DirectX.h"
#include "../DirectX/IndexBuffer.h"
#include "../DirectX/RenderTargetView.h"
#include "../DirectX/RenderTargetViewDesc.h"
#include "../DirectX/Sampler.h"
#include "../DirectX/SamplerDesc.h"
#include "../DirectX/ShaderResourceView.h"
#include "../DirectX/ShaderResourceViewDesc.h"
#include "../DirectX/SubResourceDesc.h"
#include "../DirectX/Texture2D.h"
#include "../DirectX/Texture2DDesc.h"
#include "../DirectX/Usage.h"
#include "../DirectX/VertexBuffer.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"

GBuffer::GBuffer() :
    mSampler(nullptr),
    mShader(nullptr),
    mVertexBuffer(nullptr),
    mIndexBuffer(nullptr) {
}

GBuffer::~GBuffer() = default;

void GBuffer::create() {
    Texture2DDesc desc;
    ShaderResourceViewDesc srvDesc;
    RenderTargetViewDesc rtvDesc;

    //カラー
    desc.width = Window::width();
    desc.height = Window::height();
    desc.format = Format::FORMAT_RGBA16_FLOAT;
    desc.usage = Usage::USAGE_DEFAULT;
    desc.bindFlags =
        static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_RENDER_TARGET) |
        static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_SHADER_RESOURCE);
    auto texture = std::make_shared<Texture2D>(desc);

    rtvDesc.format = desc.format;
    mRenderTargets.emplace_back(std::make_unique<RenderTargetView>(texture, &rtvDesc));

    srvDesc.format = desc.format;
    mShaderResourceViews.emplace_back(std::make_unique<ShaderResourceView>(texture, &srvDesc));

    //ノーマル
    desc.format = Format::FORMAT_R10G10B10A2_UNORM;
    auto texture2 = std::make_shared<Texture2D>(desc);

    rtvDesc.format = desc.format;
    mRenderTargets.emplace_back(std::make_unique<RenderTargetView>(texture2, &rtvDesc));

    srvDesc.format = desc.format;
    mShaderResourceViews.emplace_back(std::make_unique<ShaderResourceView>(texture2, &srvDesc));

    //ポジション
    desc.format = Format::FORMAT_RGBA16_FLOAT;
    auto texture3 = std::make_shared<Texture2D>(desc);

    rtvDesc.format = desc.format;
    mRenderTargets.emplace_back(std::make_unique<RenderTargetView>(texture3, &rtvDesc));

    srvDesc.format = desc.format;
    mShaderResourceViews.emplace_back(std::make_unique<ShaderResourceView>(texture3, &srvDesc));

    //スペキュラ
    desc.format = Format::FORMAT_RGBA16_FLOAT;
    auto texture4 = std::make_shared<Texture2D>(desc);

    rtvDesc.format = desc.format;
    mRenderTargets.emplace_back(std::make_unique<RenderTargetView>(texture4, &rtvDesc));

    srvDesc.format = desc.format;
    mShaderResourceViews.emplace_back(std::make_unique<ShaderResourceView>(texture4, &srvDesc));

    //各種生成
    createSampler();
    createShader();
    createVertexBuffer();
    createIndexBuffer();
}

void GBuffer::renderToTexture() {
    auto& dx = Singleton<DirectX>::instance();

    //各テクスチャをレンダーターゲットに設定
    static constexpr unsigned numGBuffer = static_cast<unsigned>(GBuffer::Type::NUM_GBUFFER_TEXTURES);
    ID3D11RenderTargetView* views[numGBuffer];
    for (size_t i = 0; i < numGBuffer; i++) {
        views[i] = mRenderTargets[i]->getRenderTarget();
    }
    dx.setRenderTargets(views, numGBuffer);

    //クリア
    for (size_t i = 0; i < numGBuffer; i++) {
        mRenderTargets[i]->clearRenderTarget();
    }
    dx.clearDepthStencilView();

    //デプステスト有効化
    dx.depthStencilState()->depthTest(true);
    //デプスマスク有効化
    dx.depthStencilState()->depthMask(true);
    //通常合成
    BlendDesc bd;
    bd.renderTarget.srcBlend = Blend::SRC_ALPHA;
    bd.renderTarget.destBlend = Blend::INV_SRC_ALPHA;
    dx.blendState()->setBlendState(bd);
}

void GBuffer::renderFromTexture(const Camera& camera, const DirectionalLight& dirLight, const Vector3& ambient) {
    auto& dx = Singleton<DirectX>::instance();

    //レンダーターゲットを通常に戻す
    dx.setRenderTarget();
    //クリア
    dx.clearRenderTarget();
    dx.clearDepthStencilView();

    //使用するシェーダーは、テクスチャーを参照するシェーダー
    mShader->setVSShader();
    mShader->setPSShader();
    //コンスタントバッファの登録
    mShader->setPSConstantBuffers();
    //1パス目で作成したテクスチャー3枚をセット
    setPSShaderResources();
    //サンプラーをセット
    mSampler->setPSSamplers();

    MappedSubResourceDesc msrd;
    if (mShader->map(&msrd)) {
        GBufferShaderConstantBuffer cb;
        cb.dirLightDir = dirLight.getDirection();
        cb.dirLightColor = dirLight.getLightColor();
        cb.cameraPos = camera.gameObject()->transform()->getPosition();
        cb.ambientLight = ambient;

        memcpy_s(msrd.data, msrd.rowPitch, (void*)&cb, sizeof(cb));
        mShader->unmap();
    }
    //スクリーンサイズのポリゴンをレンダー
    dx.setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_LIST);
    //バーテックスバッファーをセット
    mVertexBuffer->setVertexBuffer();
    //インデックスバッファをセット
    mIndexBuffer->setIndexBuffer(Format::FORMAT_R16_UINT);
    //デプステスト無効化
    dx.depthStencilState()->depthTest(false);

    dx.drawIndexed(6);
}

void GBuffer::setVSShaderResources() const {
    static constexpr unsigned numGBuffer = static_cast<unsigned>(GBuffer::Type::NUM_GBUFFER_TEXTURES);
    for (size_t i = 0; i < numGBuffer; i++) {
        mShaderResourceViews[i]->setVSShaderResources(i);
    }
}

void GBuffer::setPSShaderResources() const {
    static constexpr unsigned numGBuffer = static_cast<unsigned>(GBuffer::Type::NUM_GBUFFER_TEXTURES);
    for (size_t i = 0; i < numGBuffer; i++) {
        mShaderResourceViews[i]->setPSShaderResources(i);
    }
}

void GBuffer::createSampler() {
    //サンプラー作成
    SamplerDesc sd;
    mSampler = std::make_unique<Sampler>(sd);
}

void GBuffer::createShader() {
    //シェーダー生成
    mShader = Singleton<AssetsManager>::instance().createShader("Deferred.hlsl");
    mShader->createConstantBuffer(sizeof(GBufferShaderConstantBuffer));
}

void GBuffer::createVertexBuffer() {
    //バーテックスバッファ生成
    static const MeshVertex vertices[] = {
        Vector3(-1.f, -1.f, 0.f), Vector3(0.f, 0.f, -1.f), Vector2(0.f, 1.f),
        Vector3(-1.f, 1.f, 0.f), Vector3(0.f, 0.f, -1.f), Vector2(0.f, 0.f),
        Vector3(1.f, -1.f, 0.f), Vector3(0.f, 0.f, -1.f), Vector2(1.f, 1.f),
        Vector3(1.f, 1.f, 0.f), Vector3(0.f, 0.f, -1.f), Vector2(1.f, 0.f)
    };
    BufferDesc bd;
    bd.oneSize = sizeof(MeshVertex);
    bd.size = bd.oneSize * 4;
    bd.usage = Usage::USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_VERTEX);

    SubResourceDesc sub;
    sub.data = vertices;

    mVertexBuffer = std::make_unique<VertexBuffer>(bd, &sub);
}

void GBuffer::createIndexBuffer() {
    //インデックスバッファ作成
    static constexpr unsigned short indices[] = {
        0, 1, 2,
        1, 3, 2
    };
    BufferDesc bd;
    bd.size = sizeof(indices);
    bd.usage = Usage::USAGE_IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_INDEX);

    SubResourceDesc sub;
    sub.data = indices;

    mIndexBuffer = std::make_unique<IndexBuffer>(bd, &sub);
}
