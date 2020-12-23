#include "AssetsRenderTextureAdder.h"
#include "../../Device/Button.h"
#include "../../Input/Input.h"
#include "../../Math/Math.h"
#include "../../Utility/FileUtil.h"
#include "../../Utility/LevelLoader.h"

AssetsRenderTextureAdder::AssetsRenderTextureAdder()
    : mButton(nullptr)
    , mAssetsAdder(nullptr)
    , mRenderPosition()
{
}

AssetsRenderTextureAdder::~AssetsRenderTextureAdder() = default;

void AssetsRenderTextureAdder::loadProperties(const rapidjson::Value& inObj) {
    const auto& artaObj = inObj["assetsRenderTextureAdder"];
    if (artaObj.IsObject()) {
        JsonHelper::getVector2(artaObj, "renderPosition", &mRenderPosition);
        JsonHelper::getString(artaObj, "spriteButtonFilePath", &mSpriteFilePath);
    }
}

void AssetsRenderTextureAdder::initialize(IAddAssets* adder) {
    mAssetsAdder = adder;
    mButton = std::make_unique<SpriteButton>([&] { onClickButton(); }, mSpriteFilePath, mRenderPosition);
}

void AssetsRenderTextureAdder::update() {
    mButton->clickButton(Input::mouse().getMousePosition());
}

void AssetsRenderTextureAdder::draw(const Matrix4& proj) {
    mButton->draw(proj);
}

void AssetsRenderTextureAdder::onClickButton() {
    if (std::string outFilePath, outFileName; FileUtil::openFileDialog(outFilePath, outFileName)) {
        //絶対パスからアセットディレクトリ部分を抜き出す
        const auto& assetsDir = FileUtil::getAssetsFromAbsolutePath(outFilePath);
        //抜き出したアセットディレクトリからファイル名を抜いたディレクトリパスを取得する
        const auto& directoryPath = FileUtil::getDirectryFromFilePath(assetsDir);
        mAssetsAdder->add(outFileName, directoryPath);
    }
}
