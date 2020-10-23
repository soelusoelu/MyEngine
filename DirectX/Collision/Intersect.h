﻿#pragma once

#include "../Math/Math.h"
#include "../Mesh/IMesh.h"

class Ray;
class Circle;
class Sphere;
class AABB;
class Transform3D;

namespace Intersect {
    //円同士の衝突判定を行う
    bool intersectCircle(const Circle& a, const Circle& b);

    //球同士の衝突判定を行う
    bool intersectSphere(const Sphere& a, const Sphere& b);

    //AABB同士の衝突判定を行う
    bool intersectAABB(const AABB& a, const AABB& b);

    //無限平面とレイの衝突判定を行う
    bool intersectRayPlane(const Ray& ray, const Plane& p, Vector3& intersectPoint);

    //ポリゴンとレイの衝突判定を行う
    bool intersectRayPolygon(const Ray& ray, const Vector3& p1, const Vector3& p2, const Vector3& p3, Vector3& intersectPoint);

    //球とレイの衝突判定を行う
    bool intersectRaySphere(const Ray& r, const Sphere& s, float* outT);

    //AABBとレイの衝突判定を行う
    bool intersectRayAABB(const Ray& ray, const AABB& aabb, Vector3& intersectPoint);

    //メッシュとレイの衝突判定を行う
    //めちゃめちゃ重い
    bool intersectRayMesh(const Ray& ray, const IMesh& mesh, const Transform3D& transform);
};
