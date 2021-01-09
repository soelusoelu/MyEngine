#include "ModelViewCamera.h"
#include "../../Collision/Collision.h"
#include "../../Component/Engine/Camera/CameraHelper.h"
#include "../../System/Window.h"

ModelViewCamera::ModelViewCamera()
    : mModelCenterView()
    , mView()
    , mProjection()
{
}

ModelViewCamera::~ModelViewCamera() = default;

void ModelViewCamera::changeModel(const IMesh& mesh) {
    //モデルを包む球を求める
    Sphere sphere;
    SphereHelper::create(sphere, mesh);

    //モデル全体を映すビュー行列を求める
    mView = CameraHelper::getViewMatrixTakingSphereInCamera(
        sphere,
        Window::width(),
        Window::height(),
        FOV,
        Vector3::forward,
        Vector3::up
    );
    //モデル全体を映すビュー行列を保存する
    mModelCenterView = mView;

    //射影行列を求める
    mProjection = Matrix4::createPerspectiveFOV(
        Window::width(),
        Window::height(),
        FOV,
        NEAR_CLIP,
        FAR_CLIP
    );
}

void ModelViewCamera::setModelCenterPosition() {
    mView = mModelCenterView;
}

Matrix4 ModelViewCamera::getViewProjection() const {
    return mView * mProjection;
}
