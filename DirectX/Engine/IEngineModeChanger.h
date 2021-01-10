#pragma once

#include "EngineMode.h"

class IEngineModeChanger {
public:
    virtual ~IEngineModeChanger() = default;
    //エンジンモードを変更する
    virtual void change(EngineMode mode) = 0;
};
