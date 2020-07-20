#include "MeshComponent.h"
#include "../Camera/Camera.h"
#include "../../Device/AssetsManager.h"
#include "../../DirectX/DirectXInclude.h"
#include "../../GameObject/GameObject.h"
#include "../../Mesh/IMeshLoader.h"
#include "../../Mesh/Material.h"
#include "../../Mesh/MeshManager.h"
#include "../../Mesh/VertexArray.h"
#include "../../System/Shader.h"
#include "../../System/Texture.h"
#include "../../System/World.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

MeshComponent::MeshComponent() :
    Component(),
    mMesh(nullptr),
    mShader(nullptr),
    mState(State::ACTIVE),
    mCenter(Vector3::zero),
    mRadius(0.f),
    mColor(ColorPalette::white) {
}

MeshComponent::~MeshComponent() = default;

void MeshComponent::awake() {
    //半径と中心座標の取得
    mCenter = mMesh->getCenter();
    mRadius = mMesh->getRadius();

    setShader();

    setActive(gameObject()->getActive());
}

void MeshComponent::onEnable(bool value) {
    setActive(value);
}

void MeshComponent::loadProperties(const rapidjson::Value& inObj) {
    std::string fileName;
    if (JsonHelper::getString(inObj, "fileName", &fileName)) {
        setMesh(fileName);
    }
}

void MeshComponent::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("Color", mColor);
}

size_t MeshComponent::getNumMaterial() const {
    return mMaterials.size();
}

const std::shared_ptr<Material>& MeshComponent::getMaterial(unsigned index) const {
    return mMaterials[index];
}

const Vector3& MeshComponent::getCenter() const {
    return mCenter;
}

float MeshComponent::getRadius() const {
    return mRadius;
}

void MeshComponent::setColor(const Vector3& color) {
    mColor = color;
}

const Vector3& MeshComponent::getColor() const {
    return mColor;
}

void MeshComponent::destroy() {
    mState = State::DEAD;
}

void MeshComponent::setActive(bool value) {
    mState = (value) ? State::ACTIVE : State::NON_ACTIVE;
}

bool MeshComponent::getActive() const {
    return mState == State::ACTIVE;
}

bool MeshComponent::isDead() const {
    return mState == State::DEAD;
}

void MeshComponent::setMesh(const std::string& fileName) {
    mMesh = World::instance().assetsManager().createMesh(fileName);
    mMesh->setInitMaterials(&mMaterials);
    addToManager();
}

void MeshComponent::setShader() {
    mShader = World::instance().assetsManager().createShader("GBuffer.hlsl");
    //メッシュ用コンスタントバッファの作成
    mShader->createConstantBuffer(sizeof(MeshConstantBuffer), 0);
    mShader->createConstantBuffer(sizeof(MaterialConstantBuffer), 1);
    //インプットレイアウトの生成
    std::vector<InputElementDesc> layout = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, sizeof(float) * 3, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, sizeof(float) * 6, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
    };
    mShader->createInputLayout(layout);
}

void MeshComponent::draw(const Camera& camera) const {
    //使用するシェーダーの登録
    mShader->setVSShader();
    mShader->setPSShader();
    //このコンスタントバッファーを使うシェーダーの登録
    mShader->setVSConstantBuffers(0);
    mShader->setPSConstantBuffers(0);
    //頂点インプットレイアウトをセット
    mShader->setInputLayout();

    //シェーダーのコンスタントバッファーに各種データを渡す
    MappedSubResourceDesc msrd;
    if (mShader->map(&msrd, 0)) {
        MeshConstantBuffer cb;
        //ワールド行列を渡す
        cb.world = transform().getWorldTransform();
        cb.world.transpose();
        //ワールド、カメラ、射影行列を渡す
        cb.WVP = transform().getWorldTransform() * camera.getViewProjection();
        cb.WVP.transpose();

        memcpy_s(msrd.data, msrd.rowPitch, (void*)&cb, sizeof(cb));
        mShader->unmap(0);
    }

    //バーテックスバッファーをセット
    mMesh->getVertexArray()->setVertexBuffer();

    //このコンスタントバッファーを使うシェーダーの登録
    mShader->setPSConstantBuffers(1);

    //マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
    for (size_t i = 0; i < getNumMaterial(); i++) {
        //使用されていないマテリアル対策
        auto mat = getMaterial(i);
        if (mat->numIndices == 0) {
            continue;
        }
        //インデックスバッファーをセット
        mMesh->getVertexArray()->setIndexBuffer(i);

        if (mShader->map(&msrd, 1)) {
            MaterialConstantBuffer cb;
            cb.diffuse = Vector4(mat->diffuse, 1.f);
            cb.specular = Vector4(mat->specular, 1.f);

            if (auto t = mat->texture) {
                t->setPSTextures();
                t->setPSSamplers();
                cb.textureFlag = 1;
            } else {
                cb.textureFlag = 0;
            }

            memcpy_s(msrd.data, msrd.rowPitch, (void*)&cb, sizeof(cb));
            mShader->unmap(1);
        }

        //プリミティブをレンダリング
        DirectX::instance().drawIndexed(mat->numIndices);
    }
}

void MeshComponent::setMeshManager(MeshManager* manager) {
    mMeshManager = manager;
}

void MeshComponent::addToManager(bool isTransparent) {
    if (!mMeshManager) {
        return;
    }

    if (isTransparent) {
        mMeshManager->addTransparent(shared_from_this());
    } else {
        mMeshManager->add(shared_from_this());
    }
}
