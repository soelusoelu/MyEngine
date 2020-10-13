#include "LineRenderer3D.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../System/Shader.h"
#include "../../Transform/Transform3D.h"
#include <vector>

LineRenderer3D::LineRenderer3D() :
    LineRenderer(),
    mTransform(std::make_unique<Transform3D>()) {
}

LineRenderer3D::~LineRenderer3D() = default;

void LineRenderer3D::clear() {
    mLines.clear();
}

void LineRenderer3D::renderLine(const Vector3& p1, const Vector3& p2, const Vector3& color) {
    mLines.emplace_back(Line3DParam{ p1, p2, color });
}

unsigned LineRenderer3D::getParamSize() const {
    return sizeof(Line3DVertex);
}

const void* LineRenderer3D::getVertexData() const {
    static const Line3DVertex vert[] = {
        Vector3::zero, Vector3::one
    };
    return vert;
}

std::vector<InputElementDesc> LineRenderer3D::getInputLayout() const {
    std::vector<InputElementDesc> layout = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 }
    };
    return layout;
}

void LineRenderer3D::drawLines(const Matrix4& proj) const {
    for (const auto& line : mLines) {
        drawLine(line, proj);
    }
}

void LineRenderer3D::drawLine(const Line3DParam& param, const Matrix4& proj) const {
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
