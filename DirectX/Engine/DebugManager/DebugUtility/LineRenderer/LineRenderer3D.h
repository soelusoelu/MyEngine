﻿#pragma once

#include "LineRenderer.h"
#include "../../../../Math/Math.h"
#include <list>
#include <memory>

class LineRenderer3D
    : public LineRenderer
{
    struct Line3DParam {
        Vector3 p1;
        Vector3 p2;
        Vector3 color;
    };

public:
    LineRenderer3D();
    ~LineRenderer3D();

    virtual void clear() override;

    //描画したいライン情報を指定する
    void renderLine(const Vector3& p1, const Vector3& p2, const Vector3& color);

private:
    LineRenderer3D(const LineRenderer3D&) = delete;
    LineRenderer3D& operator=(const LineRenderer3D&) = delete;

    virtual std::string getShaderName() override;
    virtual void drawLines(const Matrix4& proj) const override;

    //実際にラインを描画する
    void drawLine(const Line3DParam& param, const Matrix4& proj) const;

private:
    std::list<Line3DParam> mLines;
};
