#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>

class Camera;
class DrawString;
class GBuffer;
class LightManager;

class Renderer {
public:
    Renderer();
    ~Renderer();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();

    const std::shared_ptr<DrawString>& getDrawString() const;

    void renderToTexture();
    void renderFromTexture(const Camera& camera, const LightManager& lightManager);

    //スプライト描画共通処理
    void renderSprite() const;
    //2Dスプライト処理
    void renderSprite2D(Matrix4* proj) const;
    //3Dスプライト処理
    void renderSprite3D() const;
    //デバッグ画面処理
    void renderToDebug(Matrix4* proj) const;
    //ポイントライト処理
    void renderPointLight() const;

private:
    std::shared_ptr<DrawString> mDrawString;
    std::unique_ptr<GBuffer> mGBuffer;
};
