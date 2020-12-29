#pragma once

#include "../Math/Math.h"
#include "Triangle.h"

struct RaycastHit {
    bool isHit;
    float distanceSquare;
    Vector3 point;
    Triangle polygon;

    RaycastHit()
        : isHit(false)
        , distanceSquare(FLT_MAX)
        , point()
        , polygon() {
    }
};
