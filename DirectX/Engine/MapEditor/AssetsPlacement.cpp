#include "AssetsPlacement.h"
#include "../AssetsRenderer/AssetsRenderTexture.h"
#include "../Camera/SimpleCamera.h"
#include "../../Collision/Collision.h"
#include "../../Component/Engine/Camera/Camera.h"
#include "../../Component/Engine/Mesh/MeshComponent.h"
#include "../../Component/Engine/Mesh/MeshRenderer.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/MeshGameObjectCreater.h"
#include "../../Input/Input.h"
#include "../../System/Window.h"
#include "../../System/Texture/MeshRenderOnTexture.h"
#include "../../Transform/ParentChildRelationship.h"
#include "../../Transform/Transform3D.h"
#include "../../Utility/FileUtil.h"

AssetsPlacement::AssetsPlacement()
    : mGameObjectAdder(nullptr)
    , mMeshAdder(nullptr)
    , mInspector(nullptr)
    , mTextureGetter(nullptr)
{
}

AssetsPlacement::~AssetsPlacement() = default;

void AssetsPlacement::initialize(
    IGameObjectAdder* gameObjectAdder,
    IMeshAdder* meshAdder,
    IInspectorTargetSetter* inspector,
    const ICurrentSelectTextureGetter* textureGetter
) {
    mGameObjectAdder = gameObjectAdder;
    mMeshAdder = meshAdder;
    mInspector = inspector;
    mTextureGetter = textureGetter;
}

void AssetsPlacement::placeAsset(
    const SimpleCamera& camera,
    const IMeshesGetter* meshesGetter
) {
    if (!placeConditions()) {
        return;
    }

    const auto& filePath = mTextureGetter->getCurrentSelectTexture().getTexture().getFilePath();
    //ファイルパスからファイル名を抜き出す
    const auto& fileName = FileUtil::getFileNameFromDirectry(filePath);
    //ファイル名から拡張子を取り出す
    const auto& ext = FileUtil::getFileExtension(fileName);
    //ファイル名から拡張子を抜いた部分 + _mapをゲームオブジェクトの名前とする
    auto name = fileName.substr(0, fileName.length() - ext.length()) + "_map";

    //ゲームオブジェクト生成
    auto newGameObject = std::make_shared<GameObject>();
    newGameObject->setName(name);
    mGameObjectAdder->add(newGameObject);

    //位置を決定する
    decideAssetPlaceTransform(camera, meshesGetter, newGameObject);

    //ゲームオブジェクトにメッシュをアタッチする
    auto newMesh = Component::addComponent<MeshComponent>(*newGameObject, "MeshComponent");
    newMesh->createMesh(filePath);
    //メッシュマネージャーに登録する
    mMeshAdder->add(newMesh->getComponent<MeshRenderer>(), true);

    //インスペクターの対象に設定する
    mInspector->setTarget(newGameObject);
}

void AssetsPlacement::decideAssetPlaceTransform(
    const SimpleCamera& camera,
    const IMeshesGetter* meshesGetter,
    const std::shared_ptr<GameObject>& asset
) const {
    const auto& ray = camera.screenToRay(Input::mouse().getMousePosition());
    //すべてのメッシュトレイの衝突判定
    if (RaycastHit raycastHit; Intersect::intersectRayMeshes(ray, *meshesGetter, &raycastHit)) {
        auto& hitObj = raycastHit.hitObject->transform();

        //ヒットしたオブジェクトの子に設定する
        hitObj.getParentChildRelation().addChild(asset);

        //衝突した位置に移動
        decideAssetPlacePosition(*asset, hitObj, raycastHit.point);

        //法線から角度を計算する
        decideAssetPlaceRotation(*asset, raycastHit.polygon.normal());
        return;
    }

    //どのメッシュとも衝突しなかったら、カメラの近くに適当に移動
    asset->transform().setPosition(ray.pointOnSegment(0.01f));
}

void AssetsPlacement::decideAssetPlacePosition(
    const GameObject& target,
    const Transform3D& hitObject,
    const Vector3& hitPoint
) const {
    auto pos = hitPoint - hitObject.getPosition();
    auto scale = hitObject.getScale();
    auto s = Vector3(1.f / scale.x, 1.f / scale.y, 1.f / scale.z);
    auto mat = Matrix4::createScale(s);
    auto rot = hitObject.getRotation();
    rot.conjugate();
    mat *= Matrix4::createFromQuaternion(rot);
    pos = Vector3::transform(pos, mat);
    target.transform().setPosition(pos);
}

void AssetsPlacement::decideAssetPlaceRotation(
    const GameObject& target,
    const Vector3& hitPolygonNormal
) const {
    //const auto& rot = Vector3::cross(Vector3::up, Vector3::transform(raycastHit.polygon.normal(), raycastHit.hitObject->transform().getRotation())) * 90.f;
    auto rot = Vector3::cross(Vector3::up, hitPolygonNormal) * 90.f;
    target.transform().setRotation(rot);
}

bool AssetsPlacement::placeConditions() const {
    //テクスチャが選択されていなければ終了
    if (!mTextureGetter->selectedTexture()) {
        return false;
    }

    const auto& mouse = Input::mouse();
    //マウスの左ボタンを押していなければ終了
    if (!mouse.getMouseButtonDown(MouseCode::WheelButton)) {
        return false;
    }

    const auto& mousePos = mouse.getMousePosition();
    //マウス座標がゲームウィンドウ範囲外なら終了
    if (mousePos.x > Window::width() || mousePos.y > Window::height()) {
        return false;
    }

    return true;
}
