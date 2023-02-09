#include "GentooEngine.h"

Rect::Rect () {
    x = 0;
    y = 0;
    w = 0;
    h = 0;
}

Rect::Rect (float x, float y, float w, float h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

Vec2 Rect::GetLocalCenter () {
    return Vec2(w*0.5f, h*0.5f);
}

Vec2 Rect::GetGlobalCenter () {
    return Vec2(x+w*0.5f, y+h*0.5f);
}

void Rect::SetSize (float w, float h) {
    this->w = w;
    this->h = h;
}

void Rect::SetPosition (float pX, float pY) {
    x = (pX - w*0.5f) - offset.x;
    y = (pY - h*0.5f) - offset.y;
}

void Rect::SetPosition (Vec2 point) {
    x = (point.x - w*0.5f) - offset.x;
    y = (point.y - h*0.5f) - offset.y;
}

void Rect::SetPurePosition (float pX, float pY) {
    x = pX - offset.x;
    y = pY - offset.y;
}

void Rect::SetPurePosition (Vec2 point) {
    x = point.x - offset.x;
    y = point.y - offset.y;
}

void Rect::SetBottomPosition (float pX, float pY) {
    x = (pX - w*0.5f) - offset.x;
    y = (pY - h) - offset.y;
}

void Rect::SetBottomPosition (Vec2 point) {
    x = (point.x - w*0.5f) - offset.x;
    y = (point.y - h) - offset.y;
}

void Rect::Translate (float dX, float dY) {
    x += dX;
    y += dY;
}

void Rect::Translate (Vec2 displacement) {
    x += displacement.x;
    y += displacement.y;
}

void Rect::RotateAround (Vec2 point, Vec2 axis, float angle) {
    Vec2 position = point + axis.Rotate(angle);
    SetPosition(position);
}

Vec2 Rect::GetPosition () {
    return (GetGlobalCenter() + offset);
}

bool Rect::Contains (float pX, float pY) {
    return ((pX > x) and (pX < (x+w)) and (pY > y) and (pY < (y+h)));
}

Rect Rect::GetIntersection(Rect& other) {
    Rect intersect(std::max(x, other.x), std::max(y, other.y), 0, 0);
    intersect.SetSize(std::min(other.x+other.w, x+w) - intersect.x,
        std::min(y+h, other.y+other.h) - intersect.y);
    return intersect;
}
