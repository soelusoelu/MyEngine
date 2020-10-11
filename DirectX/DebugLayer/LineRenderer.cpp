#include "LineRenderer.h"
#include "../Device/AssetsManager.h"
#include "../DirectX/DirectXInclude.h"
#include "../System/GlobalFunction.h"
#include "../System/Shader.h"
#include "../System/Window.h"
#include "../System/World.h"

LineRenderer::LineRenderer() :
    mShader(World::instance().assetsManager().createShader("Point.hlsl")) {
    if (!indexBuffer) {
        //インデックスバッファの作成
        createIndexBuffer();
    }

    mShader->createConstantBuffer(sizeof(LineConstantBuffer));
    //インプットレイアウトの生成
    std::vector<InputElementDesc> layout = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    };
    mShader->createInputLayout(layout);
}

LineRenderer::~LineRenderer() = default;

void LineRenderer::finalize() {
    safeDelete(indexBuffer);
}

void LineRenderer::drawLine2Ds(const Matrix4& proj) const {
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

std::unique_ptr<VertexBuffer> LineRenderer::createVertexBuffer(const Vector2& p1, const Vector2& p2) const {
    LineVertex vert[] = {
        p1 * 1024.f / 1920.f, p2 * 576.f / 1080.f
    };

    BufferDesc bd;
    bd.oneSize = sizeof(LineVertex);
    bd.size = bd.oneSize * 2; //頂点が2つだから
    bd.usage = Usage::USAGE_IMMUTABLE;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_VERTEX);

    SubResourceDesc sub;
    sub.data = vert;

    return std::make_unique<VertexBuffer>(bd, sub);
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
    indexBuffer = new IndexBuffer(bd, sub);
}

void LineRenderer::drawLine2D(const Line2DParam& param, const Matrix4& proj) const {
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
        LineConstantBuffer cb;

        auto vertBuf = createVertexBuffer(param.p1, param.p2);
        vertBuf->setVertexBuffer();

        //ワールド、射影行列を渡す
        cb.proj = proj;
        cb.color = Vector4(param.color, 1.f);

        memcpy_s(msrd.data, msrd.rowPitch, &cb, sizeof(cb));
        mShader->unmap();
    }
    //プリミティブをレンダリング
    DirectX::instance().drawIndexed(2);
}
