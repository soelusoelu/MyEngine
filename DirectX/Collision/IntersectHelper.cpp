﻿#include "IntersectHelper.h"
#include "RaycastHit.h"

void IntersectHelper::updateRaycastHit(
    RaycastHit& raycastHit,
    float distanceSquare,
    const Vector3& point,
    const Triangle& polygon
) {
    //既存の衝突点より手前なら更新
    if (distanceSquare < raycastHit.distanceSquare) {
        //最小記録を更新
        raycastHit.distanceSquare = distanceSquare;
        raycastHit.point = point;
        raycastHit.polygon = polygon;
        raycastHit.isHit = true;
    }
}

void IntersectHelper::updateRaycastHit(
    RaycastHit& target,
    const RaycastHit& newRaycastHit
) {
    updateRaycastHit(target, newRaycastHit.distanceSquare, newRaycastHit.point, newRaycastHit.polygon);
}
