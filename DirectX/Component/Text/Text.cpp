﻿#include "Text.h"
#include "../../Device/DrawString.h"
#include "../../Device/Renderer.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/Transform2D.h"
#include "../../Utility/LevelLoader.h"
#include "../../Utility/StringUtil.h"

Text::Text() :
    Component(),
    mText(""),
    mPosition(Vector2::zero),
    mScale(Vector2::one),
    mLightColor(ColorPalette::white),
    mAlpha(1.f),
    mPivot(Pivot::LEFT_TOP),
    mIsActive(true) {
}

Text::~Text() = default;

void Text::lateUpdate() {
    if (!mIsActive) {
        return;
    }

    auto split = StringUtil::split(mText, '\n');
    auto pos = mPosition;
    const auto& ds = gameObject()->renderer()->getDrawString();

    for (const auto& s : split) {
        ds->drawString(s, pos, mScale, mLightColor, mAlpha, mPivot);
        pos.y += DrawString::HEIGHT * mScale.y;
    }
}

void Text::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getString(inObj, "text", &mText);
    JsonHelper::getVector2(inObj, "position", &mPosition);
    JsonHelper::getVector2(inObj, "scale", &mScale);
    JsonHelper::getVector3(inObj, "color", &mLightColor);
    JsonHelper::getFloat(inObj, "alpha", &mAlpha);
    std::string src;
    if (JsonHelper::getString(inObj, "pivot", &src)) {
        PivotFunc::stringToPivot(src, &mPivot);
    }
    JsonHelper::getBool(inObj, "isActive", &mIsActive);
}

void Text::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("Text", mText);
    inspect->emplace_back("Position", mPosition);
    inspect->emplace_back("Scale", mScale);
    inspect->emplace_back("Color", mLightColor);
    inspect->emplace_back("Alpha", mAlpha);
    inspect->emplace_back("IsActive", mIsActive);
}

void Text::setText(const std::string & text) {
    mText = text;
}

const std::string& Text::text() const {
    return mText;
}

void Text::setPosition(const Vector2 & pos) {
    mPosition = pos;
}

const Vector2& Text::getPosition() const {
    return mPosition;
}

void Text::setScale(const Vector2 & scale) {
    mScale = scale;
}

const Vector2& Text::getScale() const {
    return mScale;
}

void Text::setLightColor(const Vector3 & color) {
    mLightColor = color;
}

const Vector3& Text::getLightColor() const {
    return mLightColor;
}

void Text::setAlpha(float alpha) {
    mAlpha = alpha;
}

float Text::getAlpha() const {
    return mAlpha;
}

void Text::setPivot(Pivot pivot) {
    mPivot = pivot;
}

Pivot Text::getPivot() const {
    return mPivot;
}

void Text::setActive(bool value) {
    mIsActive = value;
}

bool Text::getActive() const {
    return mIsActive;
}
