#pragma once

#include "../../Component.h"
#include "../../../System/Shader/ConstantBuffers.h"
#include <memory>

class Shader;
class RenderTexture;
class MeshRenderer;

//モデルの影を落とすクラス
class ShadowMap : public Component {
public:
    ShadowMap(GameObject& gameObject);
    ~ShadowMap();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawInspector() override;

    //描画準備
    void drawBegin(const Vector3& dirLightDirection);
    //描画
    void draw(const MeshRenderer& renderer) const;
    //影描画に使用するコンスタントバッファを登録する
    void setShadowConstantBuffer(MeshRenderer& renderer);
    //描画終了処理
    void drawEnd() const;
    //深度を書き込んだテクスチャをGPUに送る
    void transferShadowTexture(unsigned constantBufferIndex = 1);
    //レンダーターゲットテクスチャの使用後処理
    void drawEndShadowTexture();

private:
    ShadowMap(const ShadowMap&) = delete;
    ShadowMap& operator=(const ShadowMap&) = delete;

private:
    std::shared_ptr<Shader> mDepthTextureCreateShader;
    std::unique_ptr<RenderTexture> mRenderTexture;
    ShadowConstantBuffer mShadowConstBuffer;
    float mLightDistance;
    float mNearClip;
    float mFarClip;
};
