#pragma once

#include "../Mesh/IMeshLoader.h"

struct OBB;

//OBBを生成するクラス
class OBBCreater {
public:
    static OBB create(const MeshVertices& vertices);

private:
    OBBCreater() = delete;
    ~OBBCreater() = delete;
    OBBCreater(const OBBCreater&) = delete;
    OBBCreater& operator=(const OBBCreater&) = delete;

    //行列を収集する
    static Matrix3 collectMatrix(const MeshVertices& vertices);
    //固有ベクトルを求める
    static Matrix3 computeEigenvector(const Matrix3& mat);
    //行列の各成分の絶対値の最大値とその位置を求める
    static void getMaxValueForMatrix(const Matrix3& mat, float& outMax, unsigned& outRow, unsigned& outColumn);
};
