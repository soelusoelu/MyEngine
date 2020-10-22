#pragma once

#include "../Component.h"
#include "../../Math/Math.h"
#include <memory>

class Camera;

class RayMouse : public Component {
public:
    RayMouse(GameObject& gameObject);
    ~RayMouse();
    virtual void start() override;
    virtual void update() override;

private:
    std::shared_ptr<Camera> mCamera;
    Vector3 mClickPos;
};
