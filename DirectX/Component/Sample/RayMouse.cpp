#include "RayMouse.h"
#include "../Camera/Camera.h"
#include "../../DebugLayer/Debug.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Input/Input.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/Collision.h"
#include "../../Utility/StringUtil.h"

RayMouse::RayMouse(GameObject& gameObject) :
    Component(gameObject),
    mCamera(nullptr),
    mClickPos(Vector3::zero)
{
}

RayMouse::~RayMouse() = default;

void RayMouse::start() {
    mCamera = gameObject().getGameObjectManager().find("Camera")->componentManager().getComponent<Camera>();
}

void RayMouse::update() {
    //マウスインターフェイスを取得
    const auto& mouse = Input::mouse();
    //ポリゴンの各座標を設定
    auto v1 = Vector3(-10.f, 10.f, 10.f);
    auto v2 = Vector3(10.f, 10.f, 10.f);
    auto v3 = Vector3(-10.f, -10.f, 10.f);

    //マウスの左ボタンが押されていたら
    if (mouse.getMouseButton(MouseCode::LeftButton)) {
        //ワールド座標におけるマウスの位置を取得
        mClickPos = mCamera->screenToWorldPoint(mouse.getMousePosition(), 1.f);

        const auto& cameraPos = mCamera->getPosition();
        Vector3 dir = Vector3::normalize(mClickPos - cameraPos);
        Ray ray(cameraPos, dir, 10000.f);
        Vector3 out;
        //ポリゴンとレイの衝突判定
        if (intersectPolygonRay(ray, v1, v2, v3, out)) {
            transform().setPosition(out);
            Debug::log("hit");
        } else {
            Debug::log("not hit");
        }
    }

    //ポリゴンをデバッグ用に表示
    Debug::renderLine(v1, v2);
    Debug::renderLine(v2, v3);
    Debug::renderLine(v3, v1);
}
