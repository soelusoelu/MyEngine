#include "ShadowMap.h"
#include "MeshComponent.h"
#include "MeshRenderer.h"
#include "MeshShader.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"
#include "../Sprite/SpriteComponent.h"
#include "../../../DirectX/DirectXInclude.h"
#include "../../../Imgui/imgui.h"
#include "../../../Math/Math.h"
#include "../../../System/AssetsManager.h"
#include "../../../System/Window.h"
#include "../../../System/Shader/Shader.h"
#include "../../../System/Texture/Texture.h"
#include "../../../System/Texture/RenderTexture.h"
#include "../../../Transform/Transform2D.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/LevelLoader.h"

ShadowMap::ShadowMap(GameObject& gameObject)
    : Component(gameObject)
    , mDepthTextureCreateShader(AssetsManager::instance().createShader("ShadowDepthTextureCreater.hlsl"))
    , mRenderTexture(std::make_unique<RenderTexture>(Window::width(), Window::height(), Format::FORMAT_D32_FLOAT, Format::FORMAT_R16_UNORM))
    , mLightDistance(0.f)
    , mNearClip(0.f)
    , mFarClip(0.f)
{
}

ShadowMap::~ShadowMap() = default;

void ShadowMap::start() {
    //const auto& s = getComponent<SpriteComponent>();
    //const auto& tex = std::make_shared<Texture>(mRenderTexture->getShaderResourceView(), Vector2(Window::width(), Window::height()));
    //s->setTexture(tex);
    //auto& t = s->transform();
    //t.setScale(0.5f);
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
    mRenderTexture->drawBegin(1.f, 1.f, 1.f, 1.f, true, false);

    //シェーダー登録
    mDepthTextureCreateShader->setShaderInfo();

    //ライトビュー計算
    const auto& dir = dirLight.getDirection();
    mShadowConstBuffer.lightView = Matrix4::createLookAt(-dir * mLightDistance, dir, Vector3::up);
    mShadowConstBuffer.lightProj = Matrix4::createOrtho(Window::width(), Window::height(), mNearClip, mFarClip);
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
    mRenderTexture->drawEnd();
}

void ShadowMap::transferShadowTexture(unsigned constantBufferIndex) {
    mRenderTexture->transferShaderResourceView(constantBufferIndex);
}

void ShadowMap::drawEndShadowTexture() {
    mRenderTexture->drawEndTexture();
}
