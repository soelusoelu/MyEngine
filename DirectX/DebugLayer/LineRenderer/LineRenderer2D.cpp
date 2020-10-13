#include "LineRenderer2D.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../System/Shader.h"
#include "../../Transform/Transform2D.h"
#include <vector>

LineRenderer2D::LineRenderer2D() :
    LineRenderer(),
    mTransform(std::make_unique<Transform2D>()) {
    //ラインのサイズはバーテックスバッファに合わせる
    mTransform->setSize(Vector2::one);
}

LineRenderer2D::~LineRenderer2D() = default;

void LineRenderer2D::clear() {
    mLines.clear();
}

void LineRenderer2D::renderLine(const Vector2& p1, const Vector2& p2, const Vector3& color) {
    mLines.emplace_back(Line2DParam{ p1, p2, color });
}

unsigned LineRenderer2D::getParamSize() const {
    return sizeof(Line2DVertex);
}

const void* LineRenderer2D::getVertexData() const {
    static const Line2DVertex vert[] = {
        Vector2::zero, Vector2::one
    };
    return vert;
}

std::vector<InputElementDesc> LineRenderer2D::getInputLayout() const {
    std::vector<InputElementDesc> layout = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    };
    return layout;
}

void LineRenderer2D::drawLines(const Matrix4& proj) const {
    for (const auto& line : mLines) {
        drawLine(line, proj);
    }
}

void LineRenderer2D::drawLine(const Line2DParam& param, const Matrix4& proj) const {
    //シェーダーのコンスタントバッファーに各種データを渡す
    MappedSubResourceDesc msrd;
    if (mShader->map(&msrd)) {
        //パラメータからワールド行列を計算する
        mTransform->setScale(param.p2 - param.p1);
        mTransform->setPosition(param.p1);
        mTransform->computeWorldTransform();

        //シェーダーに値を渡す
        LineConstantBuffer cb;
        cb.wp = mTransform->getWorldTransform() * proj;
        cb.color = Vector4(param.color, 1.f);

        memcpy_s(msrd.data, msrd.rowPitch, &cb, sizeof(cb));
        mShader->unmap();
    }

    //描画
    DirectX::instance().drawIndexed(2);
}
