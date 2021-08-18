#pragma once

#include "EngineMode.h"
#include <functional>

class IEngineFunctionChanger {
public:
    virtual ~IEngineFunctionChanger() = default;
    //エンジンモードを変更する
    virtual void changeMode(EngineMode mode) = 0;
    //モード変更時のコールバックを設定する
    virtual void callbackChangeMode(const std::function<void(EngineMode)>& f) = 0;
};
