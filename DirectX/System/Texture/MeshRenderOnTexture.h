﻿#pragma once

#include "../../Math/Math.h"
#include <memory>
#include <string>

class RenderTexture;
class Sprite;
class Mesh;
class Shader;

//メッシュをテクスチャ上に描画する
class MeshRenderOnTexture {
public:
    MeshRenderOnTexture(const std::string& fileName, const std::string& directoryPath, int width, int height);
    ~MeshRenderOnTexture();
    //メッシュをテクスチャ上に描画する
    void drawMeshOnTexture(const Matrix4& viewProj) const;
    //メッシュ描画済みテクスチャを描画する
    void draw(const Matrix4& proj) const;
    //テクスチャの位置を設定する
    void setPositionForTexture(const Vector2& pos);

private:
    MeshRenderOnTexture(const MeshRenderOnTexture&) = delete;
    MeshRenderOnTexture& operator=(const MeshRenderOnTexture&) = delete;

private:
    std::unique_ptr<RenderTexture> mRenderTexture;
    std::unique_ptr<Sprite> mSprite;
    std::shared_ptr<Mesh> mMesh;
    std::shared_ptr<Shader> mMeshShader;
};
