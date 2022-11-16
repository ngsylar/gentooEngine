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
        void Translate(Vec2 target);
        void RotateAround(Vec2 point, Vec2 axis, float angle);

        // Access
        Vec2 GetPosition();
        bool Contains(float pX, float pY);
};

#endif