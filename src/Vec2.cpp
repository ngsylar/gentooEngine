#include "Vec2.h"

float Rad2Deg (float angle) {
    return angle * (180.0f/PI);
}

float Deg2Rad (float angleDeg) {
    return angleDeg * (PI/180.0f);
}

Vec2::Vec2 () {
    x = 0;
    y = 0;
}

Vec2::Vec2 (float x, float y) {
    this->x = x;
    this->y = y;
}

float Vec2::Magnitude () {
    return sqrt(x*x + y*y);
}

float Vec2::AngleTo (Vec2 target) {
    return atan2(target.y-y, target.x-x);
}

float Vec2::AngleDegTo (Vec2 target) {
    return AngleTo(target) * (180.0f/PI);
}

float Vec2::DistanceTo (Vec2 target) {
    target -= *this;
    return target.Magnitude();
}

Vec2 Vec2::DirectionFrom (float angle) {
    return Vec2(cos(angle), sin(angle));
}

Vec2 Vec2::Normalize () {
    float magnitude = Magnitude();
    x /= magnitude;
    y /= magnitude;
    return Vec2(x, y);
}

Vec2 Vec2::Rotate (float angle) {
    return Vec2(
        x*cos(angle) - y*sin(angle),
        x*sin(angle) + y*cos(angle)
    );
}

Vec2 Vec2::operator- () {
    return Vec2(-x, -y);
}

Vec2 Vec2::operator+ (const Vec2& vec) {
    return Vec2(x+vec.x, y+vec.y);
}

Vec2 Vec2::operator- (const Vec2& vec) {
    return Vec2(x-vec.x, y-vec.y);
}

Vec2 Vec2::operator* (const float scalar) const {
    return Vec2(x*scalar, y*scalar);
}

Vec2& Vec2::operator+= (const Vec2& vec) {
    x += vec.x;
    y += vec.y;
    return *this;
}

Vec2& Vec2::operator-= (const Vec2& vec) {
    x -= vec.x;
    y -= vec.y;
    return *this;
}

Vec2& Vec2::operator*= (const float& scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}