#include "AssetsRenderTextureAdder.h"
#include "../../Device/Button.h"
#include "../../Input/Input.h"
#include "../../Math/Math.h"
#include "../../Utility/FileUtil.h"
#include "../../Utility/JsonHelper.h"

AssetsRenderTextureAdder::AssetsRenderTextureAdder()
    : FileOperator("AssetsRenderTextureAdder")
    , mButton(nullptr)
    , mAssetsAdder(nullptr)
    , mRenderPosition()
{
}

AssetsRenderTextureAdder::~AssetsRenderTextureAdder() = default;

void AssetsRenderTextureAdder::initialize(IAddAssets* adder, IEngineFunctionChanger& changer) {
    mAssetsAdder = adder;
    mButton = std::make_unique<SpriteButton>([&] { onClickButton(); }, mSpriteFilePath, mRenderPosition);
    changer.callbackChangeMode([&](EngineMode mode) { onChangeMode(mode); });
}

void AssetsRenderTextureAdder::update() {
    mButton->clickButton(Input::mouse().getMousePosition());
}

void AssetsRenderTextureAdder::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mRenderPosition, "renderPosition", inObj, alloc, mode);
    JsonHelper::getSet(mSpriteFilePath, "spriteButtonFilePath", inObj, alloc, mode);
}

void AssetsRenderTextureAdder::onClickButton() {
    if (std::string outFilePath, outFileName; FileUtil::openFileDialog(outFilePath, outFileName)) {
        //絶対パスからアセットディレクトリ部分を抜き出す
        const auto& assetsDir = FileUtil::getAssetsFromAbsolutePath(outFilePath);
        mAssetsAdder->add(assetsDir);
    }
}

void AssetsRenderTextureAdder::onChangeMode(EngineMode mode) {
    bool isActive = (mode == EngineMode::MAP_EDITOR || mode == EngineMode::MODEL_VIEWER);
    if (mButton->getActive() != isActive) {
        mButton->setActive(isActive);
    }
}
