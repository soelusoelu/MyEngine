#include "RayMouse.h"
#include "../Camera/Camera.h"
#include "../Collider/AABBCollider.h"
#include "../Mesh/MeshComponent.h"
#include "../../Collision/Collision.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Input/Input.h"
#include "../../Math/Math.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/StringUtil.h"

RayMouse::RayMouse(GameObject& gameObject) :
    Component(gameObject),
    mCamera(nullptr),
    mMesh(nullptr),
    mAABB(nullptr) {
}

RayMouse::~RayMouse() = default;

void RayMouse::start() {
    mCamera = gameObject().getGameObjectManager().find("Camera")->componentManager().getComponent<Camera>();
    mMesh = getComponent<MeshComponent>();
    mAABB = getComponent<AABBCollider>();
    transform().setPivot(Vector3::down);
}

void RayMouse::update() {
    //マウスインターフェイスを取得
    const auto& mouse = Input::mouse();

    //マウスの左ボタンが押されていたら
    if (mouse.getMouseButton(MouseCode::LeftButton)) {
        //ワールド座標におけるマウスの位置を取得
        auto clickPos = mCamera->screenToWorldPoint(mouse.getMousePosition(), 1.f);

        //計算に必要な要素を用意しておく
        const auto& cameraPos = mCamera->getPosition();
        Vector3 dir = Vector3::normalize(clickPos - cameraPos);
        Ray ray(cameraPos, dir);
        Vector3 out;
        //メッシュとレイの衝突判定
        //(Intersect::intersectRayMesh(ray, mMesh->getMesh(), transform())) ? Debug::log("hit") : Debug::log("not hit");
        //無限平面とレイの衝突判定
        Plane plane(Vector3::up, Vector3::zero);
        if (Intersect::intersectRayPlane(ray, plane, out)) {
            //AABBとレイの衝突判定
            if (Intersect::intersectRayAABB(ray, mAABB->getAABB())) {
                transform().setPosition(out);
            }
        }
    }
}
