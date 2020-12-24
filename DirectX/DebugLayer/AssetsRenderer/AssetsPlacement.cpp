#include "AssetsPlacement.h"
#include "AssetsTexturesSelector.h"
#include "../../GameObject/MeshGameObjectCreater.h"
#include "../../Input/Input.h"
#include "../../System/Window.h"
#include "../../System/Texture/MeshRenderOnTexture.h"
#include "../../Utility/FileUtil.h"

AssetsPlacement::AssetsPlacement()
    : mSelector(std::make_unique<AssetsTexturesSelector>())
    , mCurrentSelectTexture(nullptr)
{
}

AssetsPlacement::~AssetsPlacement() = default;

void AssetsPlacement::initialize(const IAssetsRenderTexturesGetter* getter) {
    mSelector->initialize(getter);
}

void AssetsPlacement::update() {
    mSelector->selectTexture(mCurrentSelectTexture);
    placeAsset();
}

void AssetsPlacement::placeAsset() {
    if (!placeConditions()) {
        return;
    }

    const auto& filePath = mCurrentSelectTexture->getFilePath();
    //ファイルパスからファイル名を抜き出す
    const auto& fileName = FileUtil::getFileNameFromDirectry(filePath);
    //ファイル名から拡張子を取り出す
    const auto& ext = FileUtil::getFileExtension(fileName);
    //ファイル名から拡張子を抜いた部分 + _mapをゲームオブジェクトの名前とする
    auto name = fileName.substr(0, fileName.length() - ext.length()) + "_map";

    MeshGameObjectCreater::createMeshGameObject(filePath, name);
}

bool AssetsPlacement::placeConditions() const {
    //テクスチャが選択されていなければ終了
    if (!mCurrentSelectTexture) {
        return false;
    }

    const auto& mouse = Input::mouse();
    //マウスの左ボタンを押していなければ終了
    if (!mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        return false;
    }

    const auto& mousePos = mouse.getMousePosition();
    //マウス座標がゲームウィンドウ範囲外なら終了
    if (mousePos.x > Window::width() || mousePos.y > Window::height()) {
        return false;
    }

    return true;
}
