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
    mState(State::ACTIVE),
    mColor(ColorPalette::white) {
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
        //メッシュファイル名が取得できたら
        //シェーダーのデフォルトを設定
        std::string shader = "Mesh.hlsl";
        JsonHelper::getString(inObj, "shaderName", &shader);
        mMesh->loadMesh(mFileName, shader);
        addToManager();
    }
}

void MeshComponent::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("FileName", mFileName);
    inspect->emplace_back("Color", mColor);
}

const Vector3& MeshComponent::getCenter() const {
    return mMesh->getCenter();
}

float MeshComponent::getRadius() const {
    return mMesh->getRadius();
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
