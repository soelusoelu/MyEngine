﻿#pragma once

#include "IMouse.h"
#include "IMouseWheelScrollValueSetter.h"
#include "../Math/Math.h"
#include <dinput.h>
#include <string>

class Mouse
    : public IMouse
    , public IMouseWheelScrollValueSetter
{
public:
    Mouse();
    ~Mouse();
    virtual bool getMouseButtonDown(MouseCode button) const override;
    virtual bool getMouseButton(MouseCode button) const override;
    virtual bool getMouseButtonUp(MouseCode button) const override;
    virtual const Vector2& getMousePosition() const override;
    virtual const Vector2& getMouseVelocity() const override;
    virtual int getMouseScrollWheel() const override;

    virtual void setWheelScrollValue(int value) override;

    bool initialize(const HWND& hWnd, IDirectInput8* directInput);
    void update();
    void lateUpdate();
    //文字列をMouseCodeに変換
    static void stringToJoyCode(const std::string& src, MouseCode* dst);

private:
    //マウス座標の更新
    void updateMousePosition();
    //マウス座標をクランプ
    void clampMousePosition();

private:
    IDirectInputDevice8* mMouseDevice;
    DIMOUSESTATE2 mCurrentMouseState;
    DIMOUSESTATE2 mPreviousMouseState;
    HWND mhWnd;
    Vector2 mCurrentMousePosition;
    Vector2 mPreviousMousePosition;
    Vector2 mMouseVelocity;
    int mScrollWheel;
};
