﻿#include "Quaternion.h"
#include "MathUtility.h"
#include "Matrix4.h"
#include "Vector3.h"

Quaternion::Quaternion() {
    *this = Quaternion::identity;
}

Quaternion::Quaternion(float inX, float inY, float inZ, float inW) :
    x(inX),
    y(inY),
    z(inZ),
    w(inW) {
}

Quaternion::Quaternion(const Vector3& axis, float angle) {
    float scalar = Math::sin(angle / 2.f);
    x = axis.x * scalar;
    y = axis.y * scalar;
    z = axis.z * scalar;
    w = Math::cos(angle / 2.f);
}

Quaternion::Quaternion(const Vector3& euler) {
    setEuler(euler);
}

void Quaternion::set(float inX, float inY, float inZ, float inW) {
    x = inX;
    y = inY;
    z = inZ;
    w = inW;
}

Quaternion operator*(const Quaternion& q, const Quaternion& p) {
    Vector3 qv(q.x, q.y, q.z);
    Vector3 pv(p.x, p.y, p.z);
    Vector3 newVec = p.w * qv + q.w * pv + Vector3::cross(pv, qv);

    return Quaternion(
        //ベクトル部分 ps * qv + qs * pv + pv x qv
        newVec.x,
        newVec.y,
        newVec.z,
        //スカラー部分 ps * qs - pv . qv
        p.w * q.w - Vector3::dot(pv, qv)
    );
}

Quaternion& Quaternion::operator*=(const Quaternion& right) {
    *this = *this * right;
    return *this;
}

void Quaternion::conjugate() {
    x *= -1.f;
    y *= -1.f;
    z *= -1.f;
}

float Quaternion::lengthSq() const {
    return (x * x + y * y + z * z + w * w);
}

float Quaternion::length() const {
    return Math::sqrt(lengthSq());
}

void Quaternion::normalize() {
    float len = length();
    if (len == 0) {
        return;
    }
    x /= len;
    y /= len;
    z /= len;
    w /= len;
}

void Quaternion::setEuler(const Vector3& euler) {
    float cosXOver2 = Math::cos(euler.x / 2.f);
    float cosYOver2 = Math::cos(euler.y / 2.f);
    float cosZOver2 = Math::cos(euler.z / 2.f);
    float sinXOver2 = Math::sin(euler.x / 2.f);
    float sinYOver2 = Math::sin(euler.y / 2.f);
    float sinZOver2 = Math::sin(euler.z / 2.f);

    w = cosYOver2 * cosXOver2 * cosZOver2 + sinYOver2 * sinXOver2 * sinZOver2;
    x = cosYOver2 * sinXOver2 * cosZOver2 + sinYOver2 * cosXOver2 * sinZOver2;
    y = sinYOver2 * cosXOver2 * cosZOver2 - cosYOver2 * sinXOver2 * sinZOver2;
    z = cosYOver2 * cosXOver2 * sinZOver2 - sinYOver2 * sinXOver2 * cosZOver2;
}

Vector3 Quaternion::euler() const {
    Vector3 euler = Vector3::zero;
    float sp = -2.f * (y * z - w * x);
    //ジンバルロックチェック
    if (fabs(sp) > 0.9999f) {
        //真上か真下を向いている
        euler.x = Math::PIOver2 * sp;
        euler.y = atan2f(-x * z - w * y, 0.5f - y * y - z * z);
        euler.z = 0.f;
    } else {
        euler.x = asinf(sp);
        euler.y = atan2f(x * z + w * y, 0.5f - x * x - y * y);
        euler.z = atan2f(x * y + w * z, 0.5f - x * x - z * z);
    }
    euler *= Math::rad2Deg;

    return euler;
}

Quaternion Quaternion::normalize(const Quaternion& q) {
    Quaternion retVal = q;
    retVal.normalize();
    return retVal;
}

Quaternion Quaternion::lerp(const Quaternion& a, const Quaternion& b, float f) {
    Quaternion retVal;
    retVal.x = Math::lerp(a.x, b.x, f);
    retVal.y = Math::lerp(a.y, b.y, f);
    retVal.z = Math::lerp(a.z, b.z, f);
    retVal.w = Math::lerp(a.w, b.w, f);
    retVal.normalize();
    return retVal;
}

float Quaternion::dot(const Quaternion& a, const Quaternion& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Quaternion Quaternion::slerp(const Quaternion& a, const Quaternion& b, float f) {
    float rawCosm = Quaternion::dot(a, b);

    float cosom = -rawCosm;
    if (rawCosm >= 0.f) {
        cosom = rawCosm;
    }

    float scale0, scale1;

    if (cosom < 0.9999f) {
        const float omega = acosf(cosom);
        const float invSin = 1.f / sinf(omega);
        scale0 = sinf((1.f - f) * omega) * invSin;
        scale1 = sinf(f * omega) * invSin;
    } else {
        // Use linear interpolation if the quaternions
        // are collinear
        scale0 = 1.f - f;
        scale1 = f;
    }

    if (rawCosm < 0.f) {
        scale1 = -scale1;
    }

    Quaternion retVal(
        scale0 * a.x + scale1 * b.x,
        scale0 * a.y + scale1 * b.y,
        scale0 * a.z + scale1 * b.z,
        scale0 * a.w + scale1 * b.w
    );
    retVal.normalize();

    return retVal;
}

Quaternion Quaternion::lookRotation(const Vector3& forward, const Vector3& upwards) {
    const auto& z = forward;
    auto x = Vector3::normalize(Vector3::cross(upwards, z));
    auto y = Vector3::normalize(Vector3::cross(z, x));

    auto m = Matrix4::identity;
    m.m[0][0] = x.x; m.m[0][1] = x.y; m.m[0][2] = x.z;
    m.m[1][0] = y.x; m.m[1][1] = y.y; m.m[1][2] = y.z;
    m.m[2][0] = z.x; m.m[2][1] = z.y; m.m[2][2] = z.z;

    return m.getQuaternion();
}

const Quaternion Quaternion::identity(0.f, 0.f, 0.f, 1.f);
