﻿#pragma once

#include "GlobalFunction.h"
#include "../Math/Math.h"
#include <memory>
#include <vector>

class Camera;
class LightManager;
class IndexBuffer;
class RenderTargetView;
class Sampler;
class Shader;
class ShaderResourceView;
class VertexBuffer;

struct GBufferShaderConstantBuffer {
    ALIGN16 Vector3 dirLightDir; //ライト方向
    ALIGN16 Vector3 dirLightColor; //ライト色
    ALIGN16 Vector3 cameraPos; //カメラ位置
    ALIGN16 Vector3 ambientLight; //環境光
};

class GBuffer {
    enum class Type {
        DIFFUSE,
        NORMAL,
        WORLD_POS,
        SPECULAR,
        NUM_GBUFFER_TEXTURES
    };

    using SRVPtr = std::unique_ptr<ShaderResourceView>;
    using SRVPtrArray = std::vector<SRVPtr>;
    using RTVPtr = std::unique_ptr<RenderTargetView>;
    using RTVPtrArray = std::vector<RTVPtr>;

public:
    GBuffer();
    ~GBuffer();

    //Gバッファの生成
    void create();

    //GBufferテクスチャ上にレンダリング
    void renderToTexture();
    void renderFromTexture(const Camera& camera, const LightManager& lightManager);

    //全シェーダーリソースの登録
    void setVSShaderResources() const;
    void setPSShaderResources() const;

private:
    void createSampler();
    void createShader();
    void createVertexBuffer();
    void createIndexBuffer();

private:
    //Gバッファに割り当てられたテクスチャ
    RTVPtrArray mRenderTargets;
    SRVPtrArray mShaderResourceViews;
    std::unique_ptr<Sampler> mSampler;
    std::shared_ptr<Shader> mShader;
    std::unique_ptr<VertexBuffer> mVertexBuffer;
    std::unique_ptr<IndexBuffer> mIndexBuffer;
};
