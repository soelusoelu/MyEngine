#include "Renderer.h"
#include "DrawString.h"
#include "../DirectX/DirectXInclude.h"
#include "../System/GBuffer.h"
#include "../System/Texture.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"

Renderer::Renderer() :
    mDrawString(std::make_shared<DrawString>()),
    mGBuffer(std::make_unique<GBuffer>()) {
}

Renderer::~Renderer() = default;

void Renderer::loadProperties(const rapidjson::Value& inObj) {
    mDrawString->loadProperties(inObj);
}

void Renderer::initialize() {
    mGBuffer->create();
    mDrawString->initialize();
}

const std::shared_ptr<DrawString>& Renderer::getDrawString() const {
    return mDrawString;
}

void Renderer::renderToTexture() {
    Singleton<DirectX>::instance().setViewport(Window::width(), Window::height());
    mGBuffer->renderToTexture();
}

void Renderer::renderFromTexture(const Camera& camera, const LightManager& lightManager) {
    mGBuffer->renderFromTexture(camera, lightManager);
}

void Renderer::renderSprite() const {
    //プリミティブ・トポロジーをセット
    Singleton<DirectX>::instance().setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_LIST);
    //インデックスバッファーをセット
    Texture::indexBuffer->setIndexBuffer(Format::FORMAT_R16_UINT);
    //通常合成
    BlendDesc bd;
    bd.renderTarget.srcBlend = Blend::SRC_ALPHA;
    bd.renderTarget.destBlend = Blend::INV_SRC_ALPHA;
    Singleton<DirectX>::instance().blendState()->setBlendState(bd);
    //カリングオフ
    RasterizerDesc rd;
    rd.cullMode = CullMode::NONE;
    Singleton<DirectX>::instance().rasterizerState()->setRasterizerState(rd);
}

void Renderer::renderSprite2D(Matrix4* proj) const {
    //原点をスクリーン左上にするために平行移動
    proj->m[3][0] = -1.f;
    proj->m[3][1] = 1.f;
    //ピクセル単位で扱うために
    proj->m[0][0] = 2.f / Window::width();
    proj->m[1][1] = -2.f / Window::height();

    //バーテックスバッファーをセット
    Texture::vertexBuffer->setVertexBuffer();
    //デプステスト無効化
    Singleton<DirectX>::instance().depthStencilState()->depthTest(false);
}

void Renderer::renderSprite3D() const {
    //バーテックスバッファーをセット
    Texture::vertexBuffer3D->setVertexBuffer();
    //デプステスト有効化
    Singleton<DirectX>::instance().depthStencilState()->depthTest(true);
    Singleton<DirectX>::instance().depthStencilState()->depthMask(true);
}

void Renderer::renderToDebug(Matrix4* proj) const {
    Singleton<DirectX>::instance().setDebugRenderTarget();
    Singleton<DirectX>::instance().setViewport(Window::debugWidth(), Window::debugHeight());

    //原点をスクリーン左上にするために平行移動
    proj->m[3][0] = -1.f;
    proj->m[3][1] = 1.f;
    //ピクセル単位で扱うために
    proj->m[0][0] = 2.f / Window::width();
    proj->m[1][1] = -2.f / Window::height();

    //バーテックスバッファーをセット
    Texture::vertexBuffer->setVertexBuffer();
    //デプステスト無効化
    Singleton<DirectX>::instance().depthStencilState()->depthTest(false);
}

void Renderer::renderPointLight() const {
    mGBuffer->setPSShaderResources();
}
