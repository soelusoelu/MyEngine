#include "ModelViewCamera.h"
#include "../Camera/EngineCamera.h"
#include "../Camera/SimpleCamera.h"
#include "../../Collision/Collision.h"
#include "../../Component/Engine/Camera/CameraHelper.h"
#include "../../Input/Input.h"
#include "../../System/Window.h"

ModelViewCamera::ModelViewCamera()
    : mCamera(std::make_unique<EngineCamera>())
    , mModelCenterPosition()
    , mModelCetnerLookAt()
{
}

ModelViewCamera::~ModelViewCamera() = default;

void ModelViewCamera::initialize() {
    //射影行列を求め、カメラに設定する
    mCamera->getCamera().setProjection(Matrix4::createPerspectiveFOV(
        Window::width(),
        Window::height(),
        FOV,
        NEAR_CLIP,
        FAR_CLIP
    ));
}

void ModelViewCamera::update() {
    mCamera->update();

    if (Input::keyboard().getKeyDown(KeyCode::F)) {
        mCamera->initialize();
        setModelCenterPosition();
    }
}

void ModelViewCamera::onChangeModel(const IMesh& mesh) {
    //カメラ状態初期化
    mCamera->initialize();

    auto& camera = mCamera->getCamera();

    //モデルを包む球を求める
    Sphere sphere;
    SphereHelper::create(sphere, mesh);

    //注視点をモデルの中心に設定する
    mModelCetnerLookAt = sphere.center;
    camera.lookAt(mModelCetnerLookAt);

    //モデル全体を映せるカメラ位置を設定する
    mModelCenterPosition = CameraHelper::getCameraPositionTakingSphereInCamera(
        sphere,
        Window::width(),
        Window::height(),
        FOV,
        Vector3::forward
    );
    camera.setPosition(mModelCenterPosition);
}

void ModelViewCamera::setModelCenterPosition() {
    auto& camera = mCamera->getCamera();
    camera.setPosition(mModelCenterPosition);
    camera.lookAt(mModelCetnerLookAt);
}

const SimpleCamera& ModelViewCamera::getCamera() const {
    return mCamera->getCamera();
}
