#include "LineRenderer.h"
#include "../Device/AssetsManager.h"
#include "../DirectX/DirectXInclude.h"
#include "../System/Shader.h"
#include "../System/Window.h"
#include "../System/World.h"
#include "../Transform/Transform2D.h"

LineRenderer::LineRenderer() :
    mShader(nullptr),
    mVertexBuffer(nullptr),
    mIndexBuffer(nullptr),
    mTransform(std::make_unique<Transform2D>()) {
}

LineRenderer::~LineRenderer() = default;

void LineRenderer::initialize() {
    mShader = World::instance().assetsManager().createShader("Point.hlsl");
    //バーテックスバッファの作成
    createVertexBuffer();
    //インデックスバッファの作成
    createIndexBuffer();
    //ラインのサイズはバーテックスバッファに合わせる
    mTransform->setSize(Vector2::one);

    mShader->createConstantBuffer(sizeof(LineConstantBuffer));
    //インプットレイアウトの生成
    std::vector<InputElementDesc> layout = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    };
    mShader->createInputLayout(layout);
}

void LineRenderer::drawLine2Ds(const Matrix4& proj) const {
    //描画共通処理は最初に済ませる
    //シェーダーを登録
    mShader->setShaderInfo();
    //バーテックスバッファーを登録
    mVertexBuffer->setVertexBuffer();
    //インデックスバッファーを登録
    mIndexBuffer->setIndexBuffer(Format::FORMAT_R16_UINT);

    for (const auto& line : mLine2Ds) {
        drawLine2D(line, proj);
    }
}

void LineRenderer::renderLine2D(const Vector2& p1, const Vector2& p2, const Vector3& color) {
    mLine2Ds.emplace_back(Line2DParam{ p1, p2, color });
}

void LineRenderer::clear() {
    mLine2Ds.clear();
}

void LineRenderer::createVertexBuffer() {
    static const LineVertex vert[] = {
        Vector2::zero, Vector2::one
    };

    BufferDesc bd;
    bd.oneSize = sizeof(LineVertex);
    bd.size = bd.oneSize * 2; //頂点が2つだから
    bd.usage = Usage::USAGE_IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_VERTEX);

    SubResourceDesc sub;
    sub.data = vert;

    mVertexBuffer = std::make_unique<VertexBuffer>(bd, sub);
}

void LineRenderer::createIndexBuffer() {
    static constexpr unsigned short indices[] = {
        0, 1
    };
    BufferDesc bd;
    bd.size = sizeof(indices);
    bd.usage = Usage::USAGE_IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_INDEX);

    SubResourceDesc sub;
    sub.data = indices;

    mIndexBuffer = std::make_unique<IndexBuffer>(bd, sub);
}

void LineRenderer::drawLine2D(const Line2DParam& param, const Matrix4& proj) const {
    //シェーダーのコンスタントバッファーに各種データを渡す
    MappedSubResourceDesc msrd;
    if (mShader->map(&msrd)) {
        //パラメータからワールド行列を計算する
        mTransform->setScale(param.p2 - param.p1);
        mTransform->setPosition(param.p1);
        mTransform->computeWorldTransform();

        LineConstantBuffer cb;
        cb.wp = mTransform->getWorldTransform() * proj;
        cb.color = Vector4(param.color, 1.f);

        memcpy_s(msrd.data, msrd.rowPitch, &cb, sizeof(cb));
        mShader->unmap();
    }
    //プリミティブをレンダリング
    DirectX::instance().drawIndexed(2);
}
