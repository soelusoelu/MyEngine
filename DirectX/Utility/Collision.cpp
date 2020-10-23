#include "Collision.h"
#include "../Mesh/IMesh.h"
#include "../Transform/Transform3D.h"
#include <algorithm>
#include <array>
#include <vector>

Ray::Ray(const Vector3& origin, const Vector3& direction, float maxDistance) :
    start(origin),
    end(direction * maxDistance) {
}

Vector3 Ray::pointOnSegment(float t) const {
    return start + (end - start) * t;
}

float Ray::minDistanceSquare(const Vector3& point) const {
    //ベクトルの準備
    Vector3 ab = end - start;
    Vector3 ba = -1.0f * ab;
    Vector3 ac = point - start;
    Vector3 bc = point - end;

    // Case 1: Aの外側
    if (Vector3::dot(ab, ac) < 0.0f) {
        return ac.lengthSq();
    }
    // Case 2: Bの外側
    else if (Vector3::dot(ba, bc) < 0.0f) {
        return bc.lengthSq();
    }
    // Case 3:ABの間
    else {
        //pを計算
        float scalar = Vector3::dot(ac, ab) / Vector3::dot(ab, ab);
        Vector3 p = scalar * ab;
        //ac - pの長さの2乗を計算
        return (ac - p).lengthSq();
    }
}

float Ray::minDistanceSquare(const Ray& s1, const Ray& s2) {
    Vector3   u = s1.end - s1.start;
    Vector3   v = s2.end - s2.start;
    Vector3   w = s1.start - s2.start;
    float    a = Vector3::dot(u, u);         // always >= 0
    float    b = Vector3::dot(u, v);
    float    c = Vector3::dot(v, v);         // always >= 0
    float    d = Vector3::dot(u, w);
    float    e = Vector3::dot(v, w);
    float    D = a * c - b * b;        // always >= 0
    float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
    float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

                                   // compute the line parameters of the two closest points
    if (Math::nearZero(D)) { // the lines are almost parallel
        sN = 0.0;         // force using point P0 on segment S1
        sD = 1.0;         // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    } else {                 // get the closest points on the infinite lines
        sN = (b * e - c * d);
        tN = (a * e - b * d);
        if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        } else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < 0.0)
            sN = 0.0;
        else if (-d > a)
            sN = sD;
        else {
            sN = -d;
            sD = a;
        }
    } else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < 0.0)
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else {
            sN = (-d + b);
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (Math::nearZero(sN) ? 0.0f : sN / sD);
    tc = (Math::nearZero(tN) ? 0.0f : tN / tD);

    // get the difference of the two closest points
    Vector3   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)

    return dP.lengthSq();   // return the closest distance squared
}



Circle::Circle() :
    center(Vector2::zero),
    radius(0.f) {
}

Circle::Circle(const Vector2& center, float radius) :
    center(center),
    radius(radius) {
}

void Circle::set(const Vector2& center, float radius) {
    this->center = center;
    this->radius = radius;
}

bool Circle::contains(const Vector2& point) const {
    //中心と点との距離の2乗を求める
    float distSq = (center - point).lengthSq();
    return distSq <= (radius * radius);
}



Sphere::Sphere() :
    center(Vector3::zero),
    radius(0.f) {
}

Sphere::Sphere(const Vector3& center, float radius) :
    center(center),
    radius(radius) {
}

void Sphere::set(const Vector3& center, float radius) {
    this->center = center;
    this->radius = radius;
}

bool Sphere::contains(const Vector3& point) const {
    //中心と点との距離の2乗を求める
    float distSq = (center - point).lengthSq();
    return distSq <= (radius * radius);
}



AABB::AABB() :
    min(Vector3::zero),
    max(Vector3::one) {
}

AABB::AABB(const Vector3& min, const Vector3& max) :
    min(min),
    max(max) {
}

void AABB::updateMinMax(const Vector3& point) {
    min.x = Math::Min(min.x, point.x);
    min.y = Math::Min(min.y, point.y);
    min.z = Math::Min(min.z, point.z);

    max.x = Math::Max(max.x, point.x);
    max.y = Math::Max(max.y, point.y);
    max.z = Math::Max(max.z, point.z);
}

void AABB::rotate(const Quaternion& q) {
    //ボックスの角の8つの点を格納する
    std::array<Vector3, 8> points;
    //最小点からボックスの角の点を計算していく
    points[0] = min;
    points[1] = Vector3(max.x, min.y, min.z);
    points[2] = Vector3(min.x, max.y, min.z);
    points[3] = Vector3(min.x, min.y, max.z);
    points[4] = Vector3(min.x, max.y, max.z);
    points[5] = Vector3(max.x, min.y, max.z);
    points[6] = Vector3(max.x, max.y, min.z);
    points[7] = max;

    //最初に最小の点を回転させる
    auto p = Vector3::transform(points[0], q);
    //最小、最大点を回転した点に設定
    min = p;
    max = p;
    //回転した点に基づいて最小、最大点を更新する
    for (size_t i = 1; i < points.size(); ++i) {
        p = Vector3::transform(points[i], q);
        updateMinMax(p);
    }
}

bool AABB::contains(const Vector3& point) const {
    bool outside = (
        point.x < min.x ||
        point.y < min.y ||
        point.z < min.z ||
        point.x > max.x ||
        point.y > max.y ||
        point.z > max.z
    );
    //いずれにも当てはまらなければ内側
    return !outside;
}

float AABB::minDistanceSquare(const Vector3& point) const {
    //各軸の差を計算する
    float dx = Math::Max(min.x - point.x, 0.f);
    dx = Math::Max(dx, point.x - max.x);
    float dy = Math::Max(min.y - point.y, 0.f);
    dy = Math::Max(dy, point.y - max.y);
    float dz = Math::Max(min.z - point.z, 0.f);
    dz = Math::Max(dy, point.z - max.z);
    //距離の2乗
    return (dx * dx + dy * dy + dz * dz);
}



bool Intersect::intersectCircle(const Circle& a, const Circle& b) {
    Vector2 dist = a.center - b.center;
    float distSq = dist.lengthSq();
    float sumRadius = a.radius + b.radius;
    return distSq <= (sumRadius * sumRadius);
}

bool Intersect::intersectSphere(const Sphere& a, const Sphere& b) {
    Vector3 dist = a.center - b.center;
    float distSq = dist.lengthSq();
    float sumRadii = a.radius + b.radius;
    return distSq <= (sumRadii * sumRadii);
}

bool Intersect::intersectAABB(const AABB& a, const AABB& b) {
    bool no = (
        a.max.x < b.min.x ||
        a.max.y < b.min.y ||
        a.max.z < b.min.z ||
        b.max.x < a.min.x ||
        b.max.y < a.min.y ||
        b.max.z < a.min.z
    );
    //いずれにも当てはまらなければ衝突している
    return !no;
}

bool Intersect::intersectRayPlane(const Ray& ray, const Plane& p, Vector3& intersectPoint) {
    //tの解決策があるかどうかの最初のテスト
    float denom = Vector3::dot(ray.end - ray.start, p.normal());
    if (Math::nearZero(denom)) {
        //交差するのは、開始が平面上の点(P dot N) == dである場合だけ
        return (Math::nearZero(Vector3::dot(ray.start, p.normal()) - p.d));
    }

    float numer = -Vector3::dot(ray.start, p.normal()) - p.d;
    float t = numer / denom;
    //tが線分の範囲内にあるか
    if (t >= 0.f && t <= 1.f) {
        //衝突点を取得する
        intersectPoint = ray.pointOnSegment(t);
        return true;
    }

    //衝突していない
    return false;
}

bool Intersect::intersectRayPolygon(const Ray& ray, const Vector3& p1, const Vector3& p2, const Vector3& p3, Vector3& intersectPoint) {
    //まずは無限平面でテストする
    Plane plane(p1, p2, p3);
    if (!intersectRayPlane(ray, plane, intersectPoint)) {
        return false;
    }

    //各辺ベクトルを求める
    auto ab = p2 - p1;
    auto bc = p3 - p2;
    auto ca = p1 - p3;

    //各辺ベクトルと各頂点から交点へ向かうベクトルの外積を求める
    auto crossAB = Vector3::cross(ab, intersectPoint - p1);
    auto crossBC = Vector3::cross(bc, intersectPoint - p2);
    auto crossCA = Vector3::cross(ca, intersectPoint - p3);

    //それぞれの外積ベクトルとの内積を計算する
    auto dotAB = Vector3::dot(plane.normal(), crossAB);
    auto dotBC = Vector3::dot(plane.normal(), crossBC);
    auto dotCA = Vector3::dot(plane.normal(), crossCA);

    //3つの内積結果のうち1つでもマイナス符号のものがあれば交点は外にある
    return !(dotAB < 0.f || dotBC < 0.f || dotCA < 0.f);
}

bool Intersect::intersectRaySphere(const Ray& r, const Sphere& s, float* outT) {
    //方程式のX, Y, a, b, cを計算
    Vector3 X = r.start - s.center;
    Vector3 Y = r.end - r.start;
    float a = Vector3::dot(Y, Y);
    float b = 2.0f * Vector3::dot(X, Y);
    float c = Vector3::dot(X, X) - s.radius * s.radius;
    //判別式を計算
    float disc = b * b - 4.0f * a * c;
    if (disc < 0.0f) {
        return false;
    } else {
        disc = Math::sqrt(disc);
        //tの解(minとmax)を求める
        float tMin = (-b - disc) / (2.0f * a);
        float tMax = (-b + disc) / (2.0f * a);
        //tが線分の領域にあるのかチェック
        if (0.f <= tMin && tMin <= 1.0f) {
            *outT = tMin;
            return true;
        } else if (0.f <= tMax && tMax <= 1.0f) {
            *outT = tMax;
            return true;
        } else {
            return false;
        }
    }
}

bool testSidePlane(float start, float end, float negd, std::vector<float>& out) {
    float denom = end - start;
    if (Math::nearZero(denom)) {
        return false;
    }

    float numer = -start + negd;
    float t = numer / denom;
    //tが線分の範囲内にあるか
    if (t >= 0.f && t <= 1.f) {
        out.emplace_back(t);
        return true;
    } 

    //範囲外
    return false;
}

bool Intersect::intersectRayAABB(const Ray& ray, const AABB& aabb, Vector3& intersectPoint) {
    //すべてのt値を格納する
    std::vector<float> tValues;
    //x平面テスト
    testSidePlane(ray.start.x, ray.end.x, aabb.min.x, tValues);
    testSidePlane(ray.start.x, ray.end.x, aabb.max.x, tValues);
    //y平面テスト
    testSidePlane(ray.start.y, ray.end.y, aabb.min.y, tValues);
    testSidePlane(ray.start.y, ray.end.y, aabb.max.y, tValues);
    //z平面テスト
    testSidePlane(ray.start.z, ray.end.z, aabb.min.z, tValues);
    testSidePlane(ray.start.z, ray.end.z, aabb.max.z, tValues);

    //t値を昇順で並べ替える
    //std::sort(tValues.begin(), tValues.end(), [](
    //    const std::pair<float, Vector3>& a,
    //    const std::pair<float, Vector3>& b) {
    //        return a.first < b.first;
    //    });

    //ボックスに交点が含まれているか調べる
    for (const auto& t : tValues) {
        auto point = ray.pointOnSegment(t);
        if (aabb.contains(point)) {
            intersectPoint = point;
            return true;
        }
    }

    //衝突していない
    return false;
}

bool Intersect::intersectRayMesh(const Ray& ray, const IMesh& mesh, const Transform3D& transform) {
    //ワールド行列を先に取得しておく
    const auto& world = transform.getWorldTransform();

    //すべてのメッシュとレイによる判定を行う
    const auto& meshesVertices = mesh.getMeshesVertices();
    for (size_t i = 0; i < mesh.getMeshCount(); ++i) {
        const auto& meshVertices = meshesVertices[i];
        const auto polygonCount = meshVertices.size() / 3;
        for (size_t j = 0; j < polygonCount; ++j) {
            //それぞれの頂点にワールド行列を掛ける
            auto p1 = Vector3::transform(meshVertices[j * 3].pos, world);
            auto p2 = Vector3::transform(meshVertices[j * 3 + 1].pos, world);
            auto p3 = Vector3::transform(meshVertices[j * 3 + 2].pos, world);

            //同じ頂点が入っていることが有るから強制的に
            if (Math::equal(p1.x, p2.x) || Math::equal(p2.x, p3.x) || Math::equal(p3.x, p1.x)) {
                continue;
            }

            //ポリゴンとレイの衝突判定
            Vector3 out;
            if (Intersect::intersectRayPolygon(ray, p1, p2, p3, out)) {
                return true;
            }
        }
    }

    //衝突していない
    return false;
}
