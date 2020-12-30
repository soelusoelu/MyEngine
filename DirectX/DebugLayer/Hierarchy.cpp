#include "Hierarchy.h"
#include "DebugUtility.h"
#include "ImGuiInspector.h"
#include "Inspector.h"
#include "../Device/Button.h"
#include "../Device/DrawString.h"
#include "../GameObject/GameObject.h"
#include "../Input/Input.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"

Hierarchy::Hierarchy() :
    mNumRowsToDisplay(0),
    mLineSpace(0.f),
    mInspectorPositionX(0.f),
    mPosition(Vector2::zero),
    mScale(Vector2::one),
    mOffsetCharCountX(0),
    mOffsetCharCountY(0),
    mCharWidth(0.f),
    mCharHeight(0.f),
    mNonActiveAlpha(0.5f) {
}

Hierarchy::~Hierarchy() = default;

void Hierarchy::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["hierarchy"];
    if (obj.IsObject()) {
        JsonHelper::getVector2(obj, "scale", &mScale);
        JsonHelper::getInt(obj, "offsetCharCountX", &mOffsetCharCountX);
        JsonHelper::getInt(obj, "offsetCharCountY", &mOffsetCharCountY);
        JsonHelper::getFloat(obj, "lineSpace", &mLineSpace);
        JsonHelper::getFloat(obj, "nonActiveAlpha", &mNonActiveAlpha);
    }
    const auto& inspector = inObj["inspector"];
    if (inspector.IsObject()) {
        JsonHelper::getFloat(inspector, "inspectorPositionX", &mInspectorPositionX);
    }
}

void Hierarchy::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    rapidjson::Value props(rapidjson::kObjectType);
    JsonHelper::setVector2(alloc, &props, "scale", mScale);
    JsonHelper::setInt(alloc, &props, "offsetCharCountX", mOffsetCharCountX);
    JsonHelper::setInt(alloc, &props, "offsetCharCountY", mOffsetCharCountY);
    JsonHelper::setFloat(alloc, &props, "lineSpace", mLineSpace);
    JsonHelper::setFloat(alloc, &props, "nonActiveAlpha", mNonActiveAlpha);

    inObj.AddMember("hierarchy", props, alloc);
}

void Hierarchy::initialize() {
    mCharWidth = DrawString::WIDTH * mScale.x;
    mCharHeight = DrawString::HEIGHT * mScale.y;

    mPosition = Vector2(Window::width(), 0.f);
    mPosition += Vector2(mOffsetCharCountX * mCharWidth, mOffsetCharCountY * mCharHeight);

    //画面内に収まる行数
    mNumRowsToDisplay = (Window::debugHeight() - mPosition.y) / (mCharHeight + mLineSpace);

    mButtons.resize(mNumRowsToDisplay);
    auto pos = mPosition;
    for (auto&& b : mButtons) {
        //全ボタンに当たり判定をつける
        b.first = std::make_unique<Button>(nullptr, pos, Vector2(mInspectorPositionX - pos.x, mCharHeight));
        b.second = nullptr;
        pos.y += mCharHeight + mLineSpace;
    }
}

void Hierarchy::setGameObjectToButton(const GameObjectPtrList& gameObjects) {
    for (auto&& b : mButtons) {
        b.second.reset();
    }

    auto itr = mButtons.begin();
    for (const auto& obj : gameObjects) {
        //オブジェクトの数がボタンの数より多いときは無視
        if (itr == mButtons.end()) {
            return;
        }
        itr->second = obj;
        ++itr;
    }
}

void Hierarchy::update() {
    const auto& mousePos = Input::mouse().getMousePosition();
    if (Input::mouse().getMouseButtonDown(MouseCode::LeftButton)) {
        for (const auto& b : mButtons) {
            if (!b.first->containsPoint(mousePos)) {
                continue;
            }
            if (b.second) {
                DebugUtility::instance().inspector().setTarget(b.second);
                break;
            }
        }
    }
}

void Hierarchy::drawGameObjects(DrawString& drawString) const {
    for (const auto& b : mButtons) {
        auto obj = b.second;
        //オブジェクトが登録されてなかったら終了
        if (!obj) {
            break;
        }

        float alpha = 1.f;
        if (!obj->getActive()) {
            alpha = mNonActiveAlpha;
        }
        drawString.drawString(obj->name(), b.first->getPosition(), mScale, ColorPalette::white, alpha);
    }
}
