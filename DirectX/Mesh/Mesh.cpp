#include "Mesh.h"
#include "../DebugLayer/Debug.h"
#include "../Device/AssetsManager.h"
#include "../DirectX/DirectXInclude.h"
#include "../System/TextureFromFile.h"
#include "../System/World.h"
#include "../System/Shader/Shader.h"

Mesh::Mesh() :
    mMesh(nullptr),
    mShader(nullptr),
    mVertexBuffer(nullptr),
    mIndexBuffer(nullptr),
    mCenter(Vector3::zero),
    mRadius(0.f) {
}

Mesh::~Mesh() = default;

void Mesh::loadMesh(const std::string& fileName) {
    //すでに生成済みなら終了する
    if (mMesh) {
        return;
    }

    initialize(fileName);
}

void Mesh::loadShader(const std::string& shaderName) {
    createShader(shaderName);
}

void Mesh::setShaderData(const void* data, unsigned size, unsigned index) const {
    //シェーダーにデータを転送する
    mShader->transferData(data, size, index);
}

void Mesh::draw() const {
    //使用するシェーダーの登録
    mShader->setShaderInfo();
    //バーテックスバッファーをセット
    mVertexBuffer->setVertexBuffer();
    //インデックスバッファーをセット
    mIndexBuffer->setIndexBuffer();

    //マテリアルを使用していて
    if (mMesh->isUseMaterial()) {
        const auto& mat = mMesh->getMaterial();
        //テクスチャが有るなら登録
        if (mat.texture) {
            mat.texture->setTextureInfo();
        }
    }

    //プリミティブをレンダリング
    DirectX::instance().drawIndexed(mMesh->getIndices().size());
}

const Material& Mesh::getMaterial(unsigned index) const {
    return mMesh->getMaterial(index);
}

bool Mesh::isUseMaterial() const {
    return mMesh->isUseMaterial();
}

const Vector3& Mesh::getCenter() const {
    return mCenter;
}

float Mesh::getRadius() const {
    return mRadius;
}

void Mesh::initialize(const std::string& fileName) {
    createMesh(fileName);
    createVertexBuffer();
    createIndexBuffer();
    computeCenter();
    computeRadius();
}

void Mesh::createMesh(const std::string& fileName) {
    //アセットマネージャーからメッシュを作成する
    mMesh = World::instance().assetsManager().createMesh(fileName, mVertices);
}

void Mesh::createShader(const std::string& fileName) {
    //アセットマネージャーからシェーダーを作成する
    mShader = World::instance().assetsManager().createShader(fileName);
}

void Mesh::createVertexBuffer() { 
    BufferDesc bd;
    bd.oneSize = sizeof(MeshVertex);
    bd.size = bd.oneSize * mVertices.size();
    bd.usage = Usage::USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_VERTEX);
    SubResourceDesc sub;
    sub.data = mVertices.data();

    mVertexBuffer = std::make_unique<VertexBuffer>(bd, sub);
}

void Mesh::createIndexBuffer() {
    const auto& indices = mMesh->getIndices();
    BufferDesc bd;
    bd.size = sizeof(indices[0]) * indices.size();
    bd.usage = Usage::USAGE_DEFAULT;
    bd.type = static_cast<unsigned>(BufferType::BUFFER_TYPE_INDEX);
    SubResourceDesc sub;
    sub.data = indices.data();

    mIndexBuffer = std::make_unique<IndexBuffer>(bd, sub);
}

void Mesh::computeCenter() {
    const auto& positions = mMesh->getPositions();
    auto min = Vector3::one * Math::infinity;
    auto max = Vector3::one * Math::negInfinity;
    for (size_t i = 0; i < positions.size(); i++) {
        if (positions[i].x < min.x) {
            min.x = positions[i].x;
        }
        if (positions[i].x > max.x) {
            max.x = positions[i].x;
        }
        if (positions[i].y < min.y) {
            min.y = positions[i].y;
        }
        if (positions[i].y > max.y) {
            max.y = positions[i].y;
        }
        if (positions[i].z < min.z) {
            min.z = positions[i].z;
        }
        if (positions[i].z > max.z) {
            max.z = positions[i].z;
        }
    }
    mCenter = (max + min) / 2.f;
}

void Mesh::computeRadius() {
    const auto& positions = mMesh->getPositions();
    float min = Math::infinity;
    float max = Math::negInfinity;
    for (size_t i = 0; i < positions.size(); i++) {
        if (positions[i].x < min) {
            min = positions[i].x;
        }
        if (positions[i].x > max) {
            max = positions[i].x;
        }
        if (positions[i].y < min) {
            min = positions[i].y;
        }
        if (positions[i].y > max) {
            max = positions[i].y;
        }
        if (positions[i].z < min) {
            min = positions[i].z;
        }
        if (positions[i].z > max) {
            max = positions[i].z;
        }
    }
    mRadius = (max - min) / 2.f;
}
