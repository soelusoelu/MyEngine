#include "TransparentMeshComponent.h"
#include "../ComponentManager.h"
#include "../Camera/Camera.h"
#include "../Light/DirectionalLight.h"
#include "../../Device/AssetsManager.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Mesh/Material.h"
#include "../../Mesh/Mesh.h"
#include "../../System/TextureFromFile.h"
#include "../../System/World.h"
#include "../../System/Shader/ConstantBuffers.h"
#include "../../System/Shader/Shader.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/LevelLoader.h"

TransparentMeshComponent::TransparentMeshComponent(GameObject& gameObject) :
    MeshComponent(gameObject),
    mDirLight(nullptr),
    mAlpha(1.f) {
}

TransparentMeshComponent::~TransparentMeshComponent() = default;

void TransparentMeshComponent::awake() {
    MeshComponent::awake();

    const auto& dirLight = gameObject().getGameObjectManager().find("DirectionalLight");
    mDirLight = dirLight->componentManager().getComponent<DirectionalLight>();
}

void TransparentMeshComponent::loadProperties(const rapidjson::Value & inObj) {
    MeshComponent::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "alpha", &mAlpha);
}

void TransparentMeshComponent::drawDebugInfo(ComponentDebug::DebugInfoList * inspect) const {
    MeshComponent::drawDebugInfo(inspect);

    inspect->emplace_back("Alpha", mAlpha);
}

void TransparentMeshComponent::draw(const Camera& camera) const {
    //シェーダーのコンスタントバッファーに各種データを渡す
    TransparentConstantBuffer meshcb;
    const auto& world = transform().getWorldTransform();
    meshcb.world = world;
    meshcb.wvp = world * camera.getViewProjection();
    meshcb.lightDir = mDirLight->getDirection();
    meshcb.cameraPos = camera.getPosition();
    mMesh->setShaderData(&meshcb, sizeof(meshcb), 0);

    for (size_t i = 0; i < mMesh->getMeshCount(); i++) {
        MaterialConstantBuffer matcb;
        const auto& mat = mMesh->getMaterial(i);
        matcb.ambient = mat.ambient;
        matcb.diffuse = Vector4(mat.diffuse, mat.transparency * mAlpha);
        matcb.specular = mat.specular;
        mMesh->setShaderData(&matcb, sizeof(matcb), 1);

        //描画
        mMesh->draw(i);
    }
}

void TransparentMeshComponent::setAlpha(float alpha) {
    mAlpha = alpha;
}

float TransparentMeshComponent::getAlpha() const {
    return mAlpha;
}
