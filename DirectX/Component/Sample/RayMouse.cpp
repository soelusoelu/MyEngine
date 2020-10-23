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
    mMesh(nullptr) {
}

RayMouse::~RayMouse() = default;

void RayMouse::start() {
    mCamera = gameObject().getGameObjectManager().find("Camera")->componentManager().getComponent<Camera>();
    mMesh = getComponent<MeshComponent>();
}

void RayMouse::update() {
    transform().rotate(Vector3::up, 1.f);

    //マウスインターフェイスを取得
    const auto& mouse = Input::mouse();

    //マウスの左ボタンが押されていたら
    if (mouse.getMouseButton(MouseCode::LeftButton)) {
        //ワールド座標におけるマウスの位置を取得
        auto clickPos = mCamera->screenToWorldPoint(mouse.getMousePosition(), 1.f);

        //計算に必要な要素を用意しておく
        const auto& cameraPos = mCamera->getPosition();
        Vector3 dir = Vector3::normalize(clickPos - cameraPos);
        Ray ray(cameraPos, dir, 10000.f);
        //メッシュとレイの衝突判定
        if (Intersect::intersectRayMesh(ray, mMesh->getMesh(), transform())) {
            Debug::log("hit");
        } else {
            Debug::log("not hit");
        }
    }

    auto aabb = getComponent<AABBCollider>();
    Debug::renderLine(aabb->getAABB().min, aabb->getAABB().max, ColorPalette::green);
}
