#include "EngineFuctionChanger.h"
#include "../Device/Button.h"
#include "../Input/Input.h"
#include "../Utility/LevelLoader.h"

EngineFuctionChanger::EngineFuctionChanger()
    : mModeChanger(nullptr)
    , mStartRenderPosition()
    , mSpriteSpace(0.f)
{
}

EngineFuctionChanger::~EngineFuctionChanger() = default;

void EngineFuctionChanger::loadProperties(const rapidjson::Value& inObj) {
    const auto& efcObj = inObj["engineFuctionChanger"];
    if (efcObj.IsObject()) {
        JsonHelper::getVector2(efcObj, "startRenderPosition", &mStartRenderPosition);
        JsonHelper::getStringArray(efcObj, "spritesFilePath", &mSpritesFilePath);
        JsonHelper::getFloat(efcObj, "spriteSpace", &mSpriteSpace);
    }
}

void EngineFuctionChanger::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    rapidjson::Value props(rapidjson::kObjectType);
    JsonHelper::setVector2(alloc, &props, "startRenderPosition", mStartRenderPosition);
    JsonHelper::setStringArray(alloc, &props, "spritesFilePath", mSpritesFilePath);
    JsonHelper::setFloat(alloc, &props, "spriteSpace", mSpriteSpace);

    inObj.AddMember("engineFuctionChanger", props, alloc);
}

void EngineFuctionChanger::initialize(IEngineModeChanger* changer) {
    mModeChanger = changer;

    for (size_t i = 0; i < mSpritesFilePath.size(); ++i) {
        mSpritesButton.emplace_back(std::make_unique<SpriteButton>(nullptr, mSpritesFilePath[i], mStartRenderPosition + Vector2::right * (SPRITE_WIDTH + mSpriteSpace) * i));
    }
    mSpritesButton[0]->setClickFunc([&] { onClickGameButton(); });
    mSpritesButton[1]->setClickFunc([&] { onClickMapEditorButton(); });
    mSpritesButton[2]->setClickFunc([&] { onClickModelViewerButton(); });
}

void EngineFuctionChanger::update() {
    for (const auto& button : mSpritesButton) {
        button->clickButton(Input::mouse().getMousePosition());
    }
}

void EngineFuctionChanger::draw(const Matrix4& proj) const {
    for (const auto& button : mSpritesButton) {
        button->draw(proj);
    }
}

void EngineFuctionChanger::onClickGameButton() {
    mModeChanger->change(EngineMode::GAME);
}

void EngineFuctionChanger::onClickMapEditorButton() {
    mModeChanger->change(EngineMode::MAP_EDITOR);
}

void EngineFuctionChanger::onClickModelViewerButton() {
    mModeChanger->change(EngineMode::MODEL_VIEWER);
}
