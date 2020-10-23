#include "Circle.h"

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
