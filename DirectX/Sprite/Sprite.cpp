#include "Sprite.h"
#include "SpriteManager.h"
#include "../Device/AssetsManager.h"
#include "../DirectX/Buffer.h"
#include "../DirectX/DirectX.h"
#include "../DirectX/InputElementDesc.h"
#include "../DirectX/SubResourceDesc.h"
#include "../DirectX/TextureDesc.h"
#include "../GameObject/Transform2D.h"
#include "../System/Shader.h"
#include "../System/Texture.h"
#include <cassert>

Sprite::Sprite(const std::string& fileName) :
    mTransform(std::make_shared<Transform2D>()),
    mTexture(nullptr),
    mShader(Singleton<AssetsManager>::instance().createShader("Texture.hlsl")),
    mTextureSize(Vector2::zero),
    mLightColor(ColorPalette::white, 1.f),
    mUV(0.f, 0.f, 1.f, 1.f),
    mFileName(fileName),
    mIsActive(true) {

    //テクスチャ生成
    changeTexture(fileName);

    mShader->createConstantBuffer(sizeof(TextureConstantBuffer), 0);

    //インプットレイアウトの生成
    static constexpr InputElementDesc layout[] = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, sizeof(float) * 3, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
    };
    constexpr unsigned numElements = sizeof(layout) / sizeof(layout[0]);
    mShader->createInputLayout(layout, numElements);

    computeWorldTransform();
}

Sprite::~Sprite() = default;

void Sprite::computeWorldTransform() {
    if (getActive()) {
        mTransform->computeWorldTransform();
    }
}

void Sprite::draw(const Matrix4& proj) const {
    //シェーダーを登録
    mShader->setVSShader();
    mShader->setPSShader();
    //コンスタントバッファーを使うシェーダーの登録
    mShader->setVSConstantBuffers();
    mShader->setPSConstantBuffers();
    //頂点レイアウトをセット
    mShader->setInputLayout();

    //シェーダーのコンスタントバッファーに各種データを渡す
    MappedSubResourceDesc msrd;
    if (mShader->map(&msrd)) {
        TextureConstantBuffer cb;
        //ワールド、射影行列を渡す
        cb.wp = mTransform->getWorldTransform() * proj;
        cb.wp.transpose();
        cb.color = mLightColor;
        cb.uv = mUV;

        memcpy_s(msrd.data, msrd.rowPitch, (void*)(&cb), sizeof(cb));
        mShader->unmap();
    }
    //テクスチャーをシェーダーに渡す
    mTexture->setPSTextures();
    //サンプラーのセット
    mTexture->setPSSamplers();
    //プリミティブをレンダリング
    Singleton<DirectX>::instance().drawIndexed(6);
}

const std::shared_ptr<Transform2D>& Sprite::transform() const {
    return mTransform;
}

void Sprite::setLightColor(const Vector3 & color) {
    mLightColor.x = color.x;
    mLightColor.y = color.y;
    mLightColor.z = color.z;
}

void Sprite::setLightColor(float r, float g, float b) {
    mLightColor.x = r;
    mLightColor.y = g;
    mLightColor.z = b;
}

void Sprite::setAlpha(float alpha) {
    mLightColor.w = alpha;
}

const Vector4& Sprite::getLightColor() const {
    return mLightColor;
}

void Sprite::setUV(float l, float t, float r, float b) {
    assert(0.f <= l || l <= 1.f);
    assert(0.f <= t || t <= 1.f);
    assert(l <= r || r <= 1.f);
    assert(t <= b || b <= 1.f);

    mUV.x = l;
    mUV.y = t;
    mUV.z = r;
    mUV.w = b;

    //サイズ修正
    Vector2 size;
    size.x = mTextureSize.x * (r - l);
    size.y = mTextureSize.y * (b - t);

    //テクスチャサイズを変更したことを通知
    mTransform->setSize(size);
}

const Vector4& Sprite::getUV() const {
    return mUV;
}

const Vector2& Sprite::getTextureSize() const {
    return mTextureSize;
}

void Sprite::setActive(bool value) {
    mIsActive = value;
}

bool Sprite::getActive() const {
    return mIsActive;
}

void Sprite::changeTexture(const std::string& fileName) {
    if (mTexture) {
        mTexture.reset();
    }
    mTexture = Singleton<AssetsManager>::instance().createTexture(fileName);

    //デスクをもとにサイズ取得
    const auto& desc = mTexture->desc();
    mTextureSize = Vector2(desc.width, desc.height);

    //Transformに通知
    mTransform->setSize(mTextureSize);

    //ファイル名変更
    mFileName = fileName;
}

const Texture& Sprite::texture() const {
    return *mTexture;
}

const Shader& Sprite::shader() const {
    return *mShader;
}

const std::string& Sprite::fileName() const {
    return mFileName;
}
