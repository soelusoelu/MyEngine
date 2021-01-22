#include "ModelViewCamera.h"
#include "../Camera/EngineCamera.h"
#include "../Camera/SimpleCamera.h"
#include "../../Collision/Collision.h"
#include "../../Component/Engine/Camera/CameraHelper.h"
#include "../../Input/Input.h"
#include "../../System/Window.h"

ModelViewCamera::ModelViewCamera()
    : mCamera(std::make_unique<EngineCamera>())
    , mModelCenterView()
{
}

ModelViewCamera::~ModelViewCamera() = default;

void ModelViewCamera::update() {
    mCamera->update();

    if (Input::keyboard().getKeyDown(KeyCode::F)) {
        setModelCenterPosition();
    }
}

void ModelViewCamera::onChangeModel(const IMesh& mesh) {
    auto& camera = mCamera->getCamera();

    //モデルを包む球を求める
    Sphere sphere;
    SphereHelper::create(sphere, mesh);

    //モデル全体を映せるカメラ位置を設定する
    camera.setPosition(CameraHelper::getCameraPositionTakingSphereInCamera(
        sphere,
        Window::width(),
        Window::height(),
        FOV,
        Vector3::forward
    ));
    //注視点をモデルの中心に設定する
    camera.lookAt(sphere.center);

    //モデル全体を映すビュー行列を保存する
    mModelCenterView = camera.getView();

    //射影行列を求め、カメラに設定する
    camera.setProjection(Matrix4::createPerspectiveFOV(
        Window::width(),
        Window::height(),
        FOV,
        NEAR_CLIP,
        FAR_CLIP
    ));
}

void ModelViewCamera::setModelCenterPosition() {
    mCamera->getCamera().setView(mModelCenterView);
}

const SimpleCamera& ModelViewCamera::getCamera() const {
    return mCamera->getCamera();
}
