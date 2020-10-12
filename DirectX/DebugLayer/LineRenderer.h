#pragma once

#include "../Math/Math.h"
#include <memory>
#include <vector>

class VertexBuffer;
class IndexBuffer;
class Shader;
class Transform2D;

class LineRenderer {
    struct LineVertex {
        Vector2 pos;
    };

    struct LineConstantBuffer {
        Matrix4 wp;
        Vector4 color;
    };

    struct Line2DParam {
        Vector2 p1;
        Vector2 p2;
        Vector3 color;
    };

public:
    LineRenderer();
    ~LineRenderer();
    //初期化処理
    void initialize();
    //すべてのラインを描画していく
    void drawLine2Ds(const Matrix4& proj) const;
    //描画したいライン情報を指定する
    void renderLine2D(const Vector2& p1, const Vector2& p2, const Vector3& color);
    //溜まっているライン情報を削除する
    void clear();

private:
    LineRenderer(const LineRenderer&) = delete;
    LineRenderer& operator=(const LineRenderer&) = delete;

    //バーテックスバッファを作成する
    void createVertexBuffer();
    //インデックスバッファを作成する
    void createIndexBuffer();
    //実際にラインを描画する
    void drawLine2D(const Line2DParam& param, const Matrix4& proj) const;

private:
    std::vector<Line2DParam> mLine2Ds;
    std::shared_ptr<Shader> mShader;
    std::unique_ptr<VertexBuffer> mVertexBuffer;
    std::unique_ptr<IndexBuffer> mIndexBuffer;
    std::unique_ptr<Transform2D> mTransform;
};
