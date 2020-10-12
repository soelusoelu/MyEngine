#pragma once

#include "../Math/Math.h"
#include <memory>
#include <vector>

class VertexBuffer;
class IndexBuffer;
class Shader;

class LineRenderer {
    struct LineVertex {
        Vector2 pos;
    };

    struct LineConstantBuffer {
        Matrix4 proj;
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
    static void finalize();
    void drawLine2Ds(const Matrix4& proj) const;
    void renderLine2D(const Vector2& p1, const Vector2& p2, const Vector3& color);
    void clear();

private:
    LineRenderer(const LineRenderer&) = delete;
    LineRenderer& operator=(const LineRenderer&) = delete;

    std::unique_ptr<VertexBuffer> createVertexBuffer(const Vector2& p1, const Vector2& p2) const;
    void createIndexBuffer();
    void drawLine2D(const Line2DParam& param, const Matrix4& proj) const;

public:
    static inline IndexBuffer* indexBuffer = nullptr;

private:
    std::vector<Line2DParam> mLine2Ds;
    std::shared_ptr<Shader> mShader;
};
