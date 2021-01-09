#include "ModelViewer.h"
#include "ModelViewCamera.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../Component/Engine/Mesh/MeshRenderer.h"
#include "../../GameObject/GameObject.h"
#include "../../Mesh/MeshManager.h"

ModelViewer::ModelViewer()
    : mMeshManager(std::make_unique<MeshManager>(false))
    , mTargetMesh(nullptr)
    , mCamera(std::make_unique<ModelViewCamera>())
{
}

ModelViewer::~ModelViewer() = default;

void ModelViewer::initialize() {
    mMeshManager->createShadowMap();

    //auto p = std::make_unique<GameObject>();
    //auto mc = Component::addComponent<MeshComponent>(*p, "MeshComponent");
    //mc->createMesh("Plane.fbx", "Assets\\Model\\Shape\\");
    //auto mr = mc->getComponent<MeshRenderer>();
    ////床を影無しで追加
    //mMeshManager->add(mr, false);
}

void ModelViewer::setTarget(const std::shared_ptr<MeshRenderer>& target) {
    if (mTargetMesh) {
        mMeshManager->erase(mTargetMesh);
    }

    mTargetMesh = target;
    mCamera->changeModel(*target->getMeshComponent().getMesh());
}

void ModelViewer::draw(const Camera& camera, const DirectionalLight& dirLight) const {
    if (!mTargetMesh) {
        return;
    }

    mMeshManager->draw(camera, dirLight);
}
