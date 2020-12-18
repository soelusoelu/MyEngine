﻿#include "ShadowMap.h"
#include "MeshComponent.h"
#include "MeshRenderer.h"
#include "MeshShader.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"
#include "../Sprite/SpriteComponent.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../Imgui/imgui.h"
#include "../../Math/Math.h"
#include "../../System/AssetsManager.h"
#include "../../System/Window.h"
#include "../../System/Shader/Shader.h"
#include "../../System/Texture/Texture.h"
#include "../../Transform/Transform2D.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

ShadowMap::ShadowMap(GameObject& gameObject)
    : Component(gameObject)
    , mDepthTextureCreateShader(AssetsManager::instance().createShader("ShadowDepthTextureCreater.hlsl"))
    , mDepthTexture(nullptr)
    , mDepthRenderTargetView(nullptr)
    , mDepthStencilView(nullptr)
    , mDepthShaderResourceView(nullptr)
    , mWidth(0)
    , mHeight(0)
    , mLightDistance(0.f)
    , mNearClip(0.f)
    , mFarClip(0.f)
{
#ifdef _DEBUG
    mWidth = Window::debugWidth();
    mHeight = Window::debugHeight();
#else
    mWidth = Window::width();
    mHeight = Window::height();
#endif // _DEBUG
}

ShadowMap::~ShadowMap() = default;

void ShadowMap::awake() {
    Texture2DDesc desc{};
    createDepthDesc(desc);
    createDepthTexture(desc);
    createDepthRenderTargetView(desc.format);
    createDepthStencilView(desc);
    createDepthShaderResourceView(desc.format);
}

void ShadowMap::start() {
    const auto& s = getComponent<SpriteComponent>();
    const auto& tex = std::make_shared<Texture>(mDepthShaderResourceView, Vector2(mWidth, mHeight));
    s->setTexture(tex);
    auto& t = s->transform();
    t.setScale(0.3f);
}

void ShadowMap::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "lightDistance", &mLightDistance);
    JsonHelper::getFloat(inObj, "nearClip", &mNearClip);
    JsonHelper::getFloat(inObj, "farClip", &mFarClip);
}

void ShadowMap::drawInspector() {
    ImGui::DragFloat("LightDistance", &mLightDistance, 0.1f);
    ImGui::DragFloat("NearClip", &mNearClip, 0.1f, 0.f, mFarClip);
    ImGui::DragFloat("FarClip", &mFarClip, 0.1f, mNearClip, 10000.f);
}

void ShadowMap::drawBegin(const Camera& camera, const DirectionalLight& dirLight) {
    auto& dx = MyDirectX::DirectX::instance();

    //シェーダー登録
    mDepthTextureCreateShader->setShaderInfo();

    //レンダーターゲットを設定する
    mDepthRenderTargetView->setRenderTarget(*mDepthStencilView);
    //レンダーターゲットをクリアする
    mDepthRenderTargetView->clear(1.f, 1.f, 1.f, 1.f);
    //深度バッファクリア
    mDepthStencilView->clear();
    //ビューポート設定
    dx.setViewport(mWidth, mHeight);

    //ライトビュー計算
    const auto& dir = dirLight.getDirection();
    mShadowConstBuffer.lightView = Matrix4::createLookAt(-dir * mLightDistance, dir, Vector3::up);
    mShadowConstBuffer.lightProj = Matrix4::createPerspectiveFOV(mWidth, mHeight, camera.getFov(), mNearClip, mFarClip);
}

void ShadowMap::draw(const MeshRenderer& renderer) const {
    SimpleMeshConstantBuffer smcb{};
    smcb.wvp = renderer.transform().getWorldTransform() * mShadowConstBuffer.lightView * mShadowConstBuffer.lightProj;
    mDepthTextureCreateShader->transferData(&smcb, sizeof(smcb));

    const auto& meshComp = renderer.getMeshComponent();
    const auto& drawer = meshComp.getDrawer();
    const auto loopCount = meshComp.getMesh()->getMeshCount();
    for (unsigned i = 0; i < loopCount; ++i) {
        drawer->draw(i);
    }
}

void ShadowMap::setShadowConstantBuffer(MeshRenderer& renderer) {
    renderer.getMeshShader().setTransferData(&mShadowConstBuffer, sizeof(mShadowConstBuffer), 2);
}

void ShadowMap::drawEnd() const {
    auto& dx = MyDirectX::DirectX::instance();

    //レンダーターゲットをバックバッファに戻す
    dx.setRenderTarget();
    //ビューポート設定
    dx.setViewport(Window::width(), Window::height());
}

void ShadowMap::transferShadowTexture(unsigned constantBufferIndex) {
    mDepthShaderResourceView->setPSShaderResources(constantBufferIndex);
}

void ShadowMap::drawEndShadowTexture(unsigned constantBufferIndex) {
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> nullView = nullptr;
    MyDirectX::DirectX::instance().deviceContext()->PSSetShaderResources(constantBufferIndex, 1, nullView.GetAddressOf());
}

void ShadowMap::createDepthDesc(Texture2DDesc& desc) const {
    //深度テクスチャ用ディスクリプタ
    desc.width = mWidth;
    desc.height = mHeight;
    desc.format = Format::FORMAT_R16_UNORM;
    desc.usage = Usage::USAGE_DEFAULT;
    desc.bindFlags =
        static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_RENDER_TARGET)
        | static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_SHADER_RESOURCE);
}

void ShadowMap::createDepthTexture(const Texture2DDesc& desc) {
    //深度テクスチャ作成
    mDepthTexture = std::make_unique<Texture2D>(desc);
}

void ShadowMap::createDepthRenderTargetView(Format format) {
    //深度テクスチャ用レンダーターゲットビュー
    RenderTargetViewDesc rtvDesc{};
    rtvDesc.format = format;

    //深度テクスチャ用レンダーターゲットビュー作成
    mDepthRenderTargetView = std::make_unique<RenderTargetView>(*mDepthTexture, &rtvDesc);
}

void ShadowMap::createDepthStencilView(const Texture2DDesc& desc) {
    //深度テクスチャ用深度ステンシルビュー
    Texture2DDesc dsDesc{};
    memcpy_s(&dsDesc, sizeof(dsDesc), &desc, sizeof(desc));
    dsDesc.format = Format::FORMAT_D32_FLOAT;
    dsDesc.bindFlags = static_cast<unsigned>(Texture2DBind::TEXTURE_BIND_DEPTH_STENCIL);
    const auto& depthStencilTexture = std::make_unique<Texture2D>(dsDesc);

    //深度テクスチャ用深度ステンシルビュー作成
    mDepthStencilView = std::make_unique<DepthStencilView>(*depthStencilTexture);
}

void ShadowMap::createDepthShaderResourceView(Format format) {
    //深度テクスチャ用シェーダーリソースビュー
    ShaderResourceViewDesc srvDesc{};
    srvDesc.format = format;

    //深度テクスチャ用シェーダーリソースビュー作成
    mDepthShaderResourceView = std::make_shared<ShaderResourceView>(*mDepthTexture, &srvDesc);
}
