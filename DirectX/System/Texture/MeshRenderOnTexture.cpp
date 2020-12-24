﻿#include "MeshRenderOnTexture.h"
#include "RenderTexture.h"
#include "Texture.h"
#include "../AssetsManager.h"
#include "../Shader/ConstantBuffers.h"
#include "../Shader/Shader.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../Mesh/Mesh.h"
#include "../../Mesh/MeshCommonShaderSetter.h"
#include "../../Sprite/Sprite.h"
#include "../../Transform/Transform2D.h"

MeshRenderOnTexture::MeshRenderOnTexture(const std::string& filePath, int width, int height)
    : mRenderTexture(std::make_unique<RenderTexture>(width, height, Format::FORMAT_D16_UNORM, Format::FORMAT_RGBA8_UNORM))
    , mSprite(std::make_unique<Sprite>())
    , mMesh(AssetsManager::instance().createMeshFromFilePath(filePath))
    , mMeshShader(AssetsManager::instance().createShader("SimpleMeshTexture.hlsl"))
    , mFilePath(filePath)
{
    const auto& tex = std::make_shared<Texture>(mRenderTexture->getShaderResourceView(), Vector2(width, height));
    mSprite->setTexture(tex);
}

MeshRenderOnTexture::MeshRenderOnTexture(const std::string& fileName, const std::string& directoryPath, int width, int height)
    : MeshRenderOnTexture(directoryPath + fileName, width, height)
{
}

MeshRenderOnTexture::~MeshRenderOnTexture() = default;

void MeshRenderOnTexture::drawMeshOnTexture(const Matrix4& viewProj) const {
    //レンダーテクスチャへの書き込み前処理
    mRenderTexture->drawBegin();

    mMeshShader->setShaderInfo();

    SimpleMeshConstantBuffer meshcb = { Matrix4::identity * viewProj };
    mMeshShader->transferData(&meshcb, sizeof(meshcb), 0);

    for (size_t i = 0; i < mMesh->getMeshCount(); ++i) {
        MaterialConstantBuffer matcb{};
        MeshCommonShaderSetter::setMaterial(matcb, mMesh->getMaterial(i));
        mMeshShader->transferData(&matcb, sizeof(matcb), 1);

        mMesh->draw(i);
    }

    //レンダーテクスチャへの書き込み後処理
    mRenderTexture->drawEnd();
}

void MeshRenderOnTexture::draw(const Matrix4& proj) const {
    mRenderTexture->transferShaderResourceView(1);
    mSprite->draw(proj);
    mRenderTexture->drawEndTexture();
}

void MeshRenderOnTexture::setPositionForTexture(const Vector2& pos) {
    mSprite->transform().setPosition(pos);
    mSprite->computeWorldTransform();
}

const Sprite& MeshRenderOnTexture::getSprite() const {
    return *mSprite;
}

const std::string& MeshRenderOnTexture::getFilePath() const {
    return mFilePath;
}
