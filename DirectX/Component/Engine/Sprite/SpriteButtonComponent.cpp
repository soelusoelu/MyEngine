﻿#include "SpriteButtonComponent.h"
#include "../Sprite/SpriteComponent.h"
#include "../../../Device/Subject.h"
#include "../../../Imgui/imgui.h"
#include "../../../Input/Input.h"
#include "../../../Sprite/SpriteUtility.h"
#include "../../../Transform/Transform2D.h"
#include "../../../Utility/LevelLoader.h"
#include <string>

SpriteButtonComponent::SpriteButtonComponent()
    : Component()
    , mSprite(nullptr)
    , mSelectingSprite(nullptr)
    , mCallbackClick(std::make_unique<Subject>())
    , mEnableFunction(true)
    , mPreviousContains(false)
    , mWaitOneFrame(true)
{
}

SpriteButtonComponent::~SpriteButtonComponent() = default;

void SpriteButtonComponent::start() {
    mSprite = getComponent<SpriteComponent>();
    if (canAccessSprites()) {
        spriteSettings(*mSelectingSprite, *mSprite);
    }
}

void SpriteButtonComponent::update() {
    //1フレーム経過してないなら終了
    if (!mWaitOneFrame) {
        return;
    }
    //ボタン機能が無効なら何もしない
    if (!mEnableFunction) {
        return;
    }

    const auto& mouse = Input::mouse();

    auto contains = SpriteUtility::contains(*mSprite, mouse.getMousePosition());
    //スプライトの切り替え
    swapSprite(contains);

    //マウスの左ボタンを押していれば通知を送る
    if (contains && mouse.getMouseButtonDown(MouseCode::LeftButton)) {
        mCallbackClick->notify();
    }

    mPreviousContains = contains;
}

void SpriteButtonComponent::lateUpdate() {
    if (!mWaitOneFrame) {
        mWaitOneFrame = true;
    }
}

void SpriteButtonComponent::loadProperties(const rapidjson::Value& inObj) {
    if (std::string fileName;  JsonHelper::getString(inObj, "selectingSpriteFileName", &fileName)) {
        mSelectingSprite = addComponent<SpriteComponent>("SpriteComponent");
        mSelectingSprite->setTextureFromFileName(fileName);
    }
}

void SpriteButtonComponent::drawInspector() {
    ImGui::Checkbox("EnableFunction", &mEnableFunction);
}

void SpriteButtonComponent::onEnable(bool value) {
    if (value) {
        mWaitOneFrame = false;
    }
}

void SpriteButtonComponent::initialize() {
    activeSpriteSettings(mSprite, mSelectingSprite);
    mPreviousContains = false;
}

void SpriteButtonComponent::setSprite(const SpritePtr& sprite) {
    mSprite = sprite;
}

void SpriteButtonComponent::enableButtonFunction(bool value) {
    mEnableFunction = value;
    mWaitOneFrame = false;
}

void SpriteButtonComponent::callbackClick(const std::function<void()>& onClick) {
    mCallbackClick->addObserver(onClick);
}

bool SpriteButtonComponent::canAccessSprites() const {
    return (mSprite && mSelectingSprite);
}

void SpriteButtonComponent::spriteSettings(SpriteComponent& dst, const SpriteComponent& src) {
    auto& dstT = dst.transform();
    const auto& srcT = src.transform();

    dstT.setPosition(srcT.getPosition());
    dstT.setScale(srcT.getScale());
    dstT.setPivot(srcT.getPivotEnum());
    dst.setColor(src.getColor());
    dst.setAlpha(src.getAlpha());
    if (dst.getDrawOrder() != src.getDrawOrder()) {
        dst.setDrawOrder(src.getDrawOrder());
    }
}

void SpriteButtonComponent::activeSpriteSettings(const SpritePtr& active, const SpritePtr& nonActive) {
    if (active) {
        active->setActive(true);
    }
    if (nonActive) {
        nonActive->setActive(false);
    }
}

void SpriteButtonComponent::swapSprite(bool contains) {
    //スプライトにアクセスできなかったら終了
    if (!canAccessSprites()) {
        return;
    }

    if (contains && !mPreviousContains) {
        activeSpriteSettings(mSelectingSprite, mSprite);
    } else if (!contains && mPreviousContains) {
        activeSpriteSettings(mSprite, mSelectingSprite);
    }
}
