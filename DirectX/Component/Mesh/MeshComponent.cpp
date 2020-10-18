#include "MeshComponent.h"
#include "../Camera/Camera.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../Mesh/Mesh.h"
#include "../../Mesh/MeshManager.h"
#include "../../System/Shader/ConstantBuffers.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

MeshComponent::MeshComponent(GameObject& gameObject) :
    Component(gameObject),
    mMesh(std::make_unique<Mesh>()),
    mFileName(),
    mState(State::ACTIVE) {
}

MeshComponent::~MeshComponent() = default;

void MeshComponent::awake() {
    setActive(gameObject().getActive());
}

void MeshComponent::onEnable(bool value) {
    setActive(value);
}

void MeshComponent::loadProperties(const rapidjson::Value& inObj) {
    //ファイル名からメッシュを生成
    if (JsonHelper::getString(inObj, "fileName", &mFileName)) {
        mMesh->loadMesh(mFileName);
        addToManager();
    }

    std::string shader;
    //シェーダー名が取得できたら読み込む
    if (JsonHelper::getString(inObj, "shaderName", &shader)) {
        mMesh->loadShader(shader);
    } else {
        //シェーダー名が取得できなかったらデフォルトのシェーダーを使う
        shader = "Mesh.hlsl";
        //マテリアルが有るなら
        if (mMesh->isUseMaterial()) {
            //テクスチャが有るなら
            if (mMesh->getMaterial().texture) {
                shader = "MeshTexture.hlsl";
            }
        }
        mMesh->loadShader(shader);
    }
}

void MeshComponent::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("FileName", mFileName);
}

const Vector3& MeshComponent::getCenter() const {
    return mMesh->getCenter();
}

float MeshComponent::getRadius() const {
    return mMesh->getRadius();
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

void MeshComponent::setMeshManager(MeshManager* manager) {
    mMeshManager = manager;
}

void MeshComponent::addToManager() {
    //マネージャーが登録されていなかったら終了する
    if (!mMeshManager) {
        Debug::logWarning("The mesh manager is not registered.");
        return;
    }

    //マネージャーに自身を登録する
    mMeshManager->addTransparent(shared_from_this());
}
