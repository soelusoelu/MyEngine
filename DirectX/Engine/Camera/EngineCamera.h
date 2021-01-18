#pragma once

#include "../../Input/IMouse.h"
#include <memory>

class SimpleCamera;

//エンジンのデフォルトで使われるカメラ
class EngineCamera {
public:
    EngineCamera();
    ~EngineCamera();
    void update();
    SimpleCamera& getCamera() const;

private:
    EngineCamera(const EngineCamera&) = delete;
    EngineCamera& operator=(const EngineCamera&) = delete;

    //カメラを移動する
    void moveCamera(const IMouse& mouse);
    //注視点を中心にカメラを回転する
    void rotateLookAtPoint(const IMouse& mouse);

private:
    std::unique_ptr<SimpleCamera> mCamera;
};
