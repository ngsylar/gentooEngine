#ifndef RECT_H
#define RECT_H

#include "Vec2.h"

class Rect {
    public:
        float x, y, w, h;
        Vec2 offset;

        // Basic
        Rect();
        Rect(float x, float y, float w, float h);
        Vec2 GetLocalCenter();
        Vec2 GetGlobalCenter();

        // Modifier
        void SetSize(float w, float h);
        void SetPosition(float x, float y);
        void SetPosition(Vec2 point);
        void SetPurePosition(float x, float y);
        void SetPurePosition(Vec2 point);
        void SetBottomPosition(float x, float y);
        void SetBottomPosition(Vec2 point);
        void Translate(float x, float y);
        void Translate(Vec2 displacement);
        void RotateAround(Vec2 point, Vec2 axis, float angle);

        // Access
        Vec2 GetPosition();
        Rect GetIntersection(Rect& other);
        bool Contains(float pX, float pY);
};

#endif