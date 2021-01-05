#include "AssetsPlacement.h"
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
    : mInspector(nullptr)
    , mMeshesGetter(nullptr)
    , mTextureGetter(nullptr)
    , mCamera(nullptr)
{
}

AssetsPlacement::~AssetsPlacement() = default;

void AssetsPlacement::initialize(
    const std::shared_ptr<Camera>& camera,
    IInspectorTargetSetter* inspector,
    const IMeshesGetter* meshesGetter,
    const ICurrentSelectTextureGetter* textureGetter
) {
    mCamera = camera;
    mInspector = inspector;
    mMeshesGetter = meshesGetter;
    mTextureGetter = textureGetter;
}

void AssetsPlacement::update() {
    placeAsset();
}

void AssetsPlacement::placeAsset() {
    if (!placeConditions()) {
        return;
    }

    const auto& filePath = mTextureGetter->getCurrentSelectTexture().getFilePath();
    //ファイルパスからファイル名を抜き出す
    const auto& fileName = FileUtil::getFileNameFromDirectry(filePath);
    //ファイル名から拡張子を取り出す
    const auto& ext = FileUtil::getFileExtension(fileName);
    //ファイル名から拡張子を抜いた部分 + _mapをゲームオブジェクトの名前とする
    auto name = fileName.substr(0, fileName.length() - ext.length()) + "_map";

    const auto& newObj = MeshGameObjectCreater::createMeshGameObject(filePath, name);
    decideAssetPlacePosition(newObj);

    //インスペクターの対象に設定する
    mInspector->setTarget(newObj);
}

void AssetsPlacement::decideAssetPlacePosition(const std::shared_ptr<GameObject>& asset) const {
    const auto& ray = mCamera->screenToRay(Input::mouse().getMousePosition());
    //すべてのメッシュトレイの衝突判定
    if (RaycastHit raycastHit; Intersect::intersectRayMeshes(ray, *mMeshesGetter, &raycastHit)) {
        auto& hitObj = raycastHit.hitObject->transform();

        //ヒットしたオブジェクトの子に設定する
        hitObj.getParentChildRelation().addChild(asset);

        //衝突した位置に移動
        asset->transform().setPosition(-hitObj.getPosition() + Vector3::transform(raycastHit.point - hitObj.getPosition(), hitObj.getWorldTransform()));
        //法線から角度を計算する
        //const auto& rot = Vector3::cross(Vector3::up, Vector3::transform(raycastHit.polygon.normal(), raycastHit.hitObject->transform().getRotation())) * 90.f;
        //const auto& rot = Vector3::cross(Vector3::up, raycastHit.polygon.normal()) * 90.f;
        //asset->transform().setRotation(rot);
        return;
    }

    //どのメッシュとも衝突しなかったら、カメラの近くに適当に移動
    asset->transform().setPosition(ray.pointOnSegment(0.01f));
}

bool AssetsPlacement::placeConditions() const {
    //テクスチャが選択されていなければ終了
    if (!mTextureGetter->selectedTexture()) {
        return false;
    }

    const auto& mouse = Input::mouse();
    //マウスの左ボタンを押していなければ終了
    if (!mouse.getMouseButtonDown(MouseCode::CenterButton)) {
        return false;
    }

    const auto& mousePos = mouse.getMousePosition();
    //マウス座標がゲームウィンドウ範囲外なら終了
    if (mousePos.x > Window::width() || mousePos.y > Window::height()) {
        return false;
    }

    return true;
}
