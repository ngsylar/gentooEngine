#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#define PI 3.1415926535

float Rad2Deg(float angle);
float Deg2Rad(float angleDeg);

class Vec2 {
    public:
        float x, y;

        Vec2();
        Vec2(float x, float y);
        float Magnitude();
        float AngleTo(Vec2 target);
        float AngleDegTo(Vec2 target);
        float DistanceTo(Vec2 target);
        Vec2 DirectionFrom(float angle);
        
        Vec2 Normalize();
        Vec2 Rotate(float angle);

        Vec2 operator-();
        Vec2 operator+(const Vec2& vec);
        Vec2 operator-(const Vec2& vec);
        Vec2 operator*(const float scalar) const;

        Vec2& operator+=(const Vec2& vec);
        Vec2& operator-=(const Vec2& vec);
        Vec2& operator*=(const float& scalar);
};

#endif