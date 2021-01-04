#include "SceneMeshSelector.h"
#include "../../Collision/Collision.h"
#include "../../Component/Engine/Camera/Camera.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../Input/Input.h"

SceneMeshSelector::SceneMeshSelector()
    : mCamera(nullptr)
    , mMeshesGetter(nullptr)
{
}

SceneMeshSelector::~SceneMeshSelector() = default;

void SceneMeshSelector::initialize(const std::shared_ptr<Camera>& camera, const IMeshesGetter* getter) {
    mCamera = camera;
    mMeshesGetter = getter;
}

bool SceneMeshSelector::selectMesh(std::shared_ptr<MeshComponent>& out) {
    const auto& ray = mCamera->screenToRay(Input::mouse().getMousePosition());
    if (RaycastHit raycastHit{}; Intersect::intersectRayMeshes(ray, *mMeshesGetter, &raycastHit)) {
        out = raycastHit.hitObject->componentManager().getComponent<MeshComponent>();
        return true;
    }

    return false;
}
