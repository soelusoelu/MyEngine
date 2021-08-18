#include "EngineFuctionChanger.h"
#include "../Device/Button.h"
#include "../Input/Input.h"
#include "../Utility/JsonHelper.h"

EngineFuctionChanger::EngineFuctionChanger()
    : FileOperator("EngineFuctionChanger")
    , mStartRenderPosition()
    , mSpriteSpace(0.f)
{
}

EngineFuctionChanger::~EngineFuctionChanger() = default;

void EngineFuctionChanger::changeMode(EngineMode mode) {
    mCallbackChangeMode(mode);
}

void EngineFuctionChanger::callbackChangeMode(const std::function<void(EngineMode)>& f) {
    mCallbackChangeMode += f;
}

void EngineFuctionChanger::initialize() {
    for (size_t i = 0; i < mSpritesFilePath.size(); ++i) {
        mSpritesButton.emplace_back(std::make_unique<SpriteButton>(nullptr, mSpritesFilePath[i], mStartRenderPosition + Vector2::right * (SPRITE_WIDTH + mSpriteSpace) * i));
    }
}

void EngineFuctionChanger::update() {
    const auto& mousePos = Input::mouse().getMousePosition();
    for (size_t i = 0; i < mSpritesButton.size(); ++i) {
        bool result = mSpritesButton[i]->clickButton(mousePos);
        if (result) {
            mCallbackChangeMode(static_cast<EngineMode>(i));
            return;
        }
    }
}

void EngineFuctionChanger::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mStartRenderPosition, "startRenderPosition", inObj, alloc, mode);
    JsonHelper::getSet(mSpritesFilePath, "spritesFilePath", inObj, alloc, mode);
    JsonHelper::getSet(mSpriteSpace, "spriteSpace", inObj, alloc, mode);
}
