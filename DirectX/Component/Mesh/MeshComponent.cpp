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
        mMesh->loadMesh(mFileName);
    }
}

void MeshComponent::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("FileName", mFileName);
    inspect->emplace_back("Color", mColor);
}

void MeshComponent::draw(const Camera& camera) const {
    //シェーダーのコンスタントバッファーに各種データを渡す
    MeshConstantBuffer meshcb;
    meshcb.world = transform().getWorldTransform();
    meshcb.wvp = transform().getWorldTransform() * camera.getViewProjection();
    mMesh->setShaderData(&meshcb, sizeof(meshcb), 0);

    MaterialConstantBuffer matcb;
    matcb.diffuse = Vector4(mColor, 1.f);
    matcb.specular = Vector3(0.3f, 0.3f, 0.3f);
    mMesh->setShaderData(&matcb, sizeof(matcb), 1);

    //描画
    mMesh->draw();
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
    mMeshManager->add(shared_from_this());
}
