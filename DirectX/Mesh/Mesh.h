#pragma once

#include "IMeshLoader.h"
#include "../Math/Math.h"
#include <memory>
#include <string>
#include <vector>

class Shader;
class VertexBuffer;
class IndexBuffer;

class Mesh {
public:
    Mesh();
    ~Mesh();
    //ファイル名からメッシュを生成する
    void loadMesh(const std::string& fileName);
    //コンスタントバッファを設定する
    void setShaderData(const void* data, unsigned size, unsigned index = 0) const;
    //メッシュを描画する
    void draw() const;

    //中心座標の取得
    const Vector3& getCenter() const;
    //半径の取得
    float getRadius() const;

private:
    //初期化処理
    void initialize(const std::string& fileName);
    //メッシュを生成する
    void createMesh(const std::string& fileName);
    //シェーダーを生成する
    void createShader();
    //バーテックスバッファを生成する
    void createVertexBuffer();
    //インデックスバッファを生成する
    void createIndexBuffer();

private:
    std::shared_ptr<IMeshLoader> mMesh;
    std::vector<MeshVertex> mVertices;
    std::unique_ptr<Shader> mShader;
    std::unique_ptr<VertexBuffer> mVertexBuffer;
    std::unique_ptr<IndexBuffer> mIndexBuffer;

    Vector3 mCenter;
    float mRadius;
};
