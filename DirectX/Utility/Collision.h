#pragma once

#include "../Math/Math.h"
#include "../Mesh/IMesh.h"

class Transform3D;

struct Ray {
    Ray(const Vector3& origin, const Vector3& direction, float maxDistance);
    //線分上の点を返す [0, t, 1]
    Vector3 pointOnSegment(float t) const;
    //最短距離の2乗
    float minDistanceSquare(const Vector3& point) const;
    //2本の線分から最短距離の2乗を取得
    static float minDistanceSquare(const Ray& s1, const Ray& s2);

    Vector3 start;
    Vector3 end;
};

struct Circle {
    Circle();
    Circle(const Vector2& center, float radius);
    void set(const Vector2& center, float radius);
    bool contains(const Vector2& point) const;

    Vector2 center;
    float radius;
};

struct Sphere {
    Sphere();
    Sphere(const Vector3& center, float radius);
    void set(const Vector3& center, float radius);
    bool contains(const Vector3& point) const;

    Vector3 center;
    float radius;
};

struct AABB {
    AABB();
    AABB(const Vector3& min, const Vector3& max);
    void updateMinMax(const Vector3& point);
    void rotate(const Quaternion& q);
    bool contains(const Vector3& point) const;
    float minDistanceSquare(const Vector3& point) const;

    Vector3 min;
    Vector3 max;
};

//交差判定
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
}
