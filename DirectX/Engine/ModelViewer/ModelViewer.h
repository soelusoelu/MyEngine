#pragma once

#include "../../Math/Math.h"
#include <memory>

class Camera;
class DirectionalLight;
class MeshManager;
class MeshRenderer;
class ModelViewCamera;

class ModelViewer {
public:
    ModelViewer();
    ~ModelViewer();
    //初期化
    void initialize();
    //モデル対象を設定する
    void setTarget(const std::shared_ptr<MeshRenderer>& target);
    //モデルを描画する
    void draw(const Camera& camera, const DirectionalLight& dirLight) const;

private:
    ModelViewer(const ModelViewer&) = delete;
    ModelViewer& operator=(const ModelViewer&) = delete;

private:
    std::unique_ptr<MeshManager> mMeshManager;
    std::shared_ptr<MeshRenderer> mTargetMesh;
    std::unique_ptr<ModelViewCamera> mCamera;
};
