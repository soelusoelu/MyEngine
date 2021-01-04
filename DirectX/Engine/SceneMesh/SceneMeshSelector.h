#pragma once

#include "../../Mesh/IMeshesGetter.h"
#include <memory>

class Camera;
class MeshComponent;

class SceneMeshSelector {
public:
    SceneMeshSelector();
    ~SceneMeshSelector();
    void initialize(const std::shared_ptr<Camera>& camera, const IMeshesGetter* getter);
    //メッシュを選択する
    bool selectMesh(std::shared_ptr<MeshComponent>& out);

private:
    SceneMeshSelector(const SceneMeshSelector&) = delete;
    SceneMeshSelector& operator=(const SceneMeshSelector&) = delete;

private:
    std::shared_ptr<Camera> mCamera;
    const IMeshesGetter* mMeshesGetter;
};
