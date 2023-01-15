#include "GentooEngine.h"

Collider::Collider (GameObject& associated, Vec2 offset, bool trigger): Component(associated) {
    this->offset = offset;
    isTrigger = trigger;
    started = false;
    type = GameObjID::_Collider;
}

void Collider::SetBox (Vec2 offset, Vec2 boxSize) {
    this->offset = offset;
    box.SetSize(boxSize.x, boxSize.y);
    started = true;
}

void Collider::Start () {
    if (started) return;
    box.SetSize(associated.box.w, associated.box.h);
}

void Collider::Update (float dt) {
    Vec2 offsetDifference = offset - associated.box.offset;
    if (associated.angleDeg)
        offsetDifference = offsetDifference.Rotate(Deg2Rad(associated.angleDeg));
    box.SetPosition(associated.box.GetPosition() + offsetDifference);
}

// DEBUG
void Collider::Render () {
    if (not Game::GetInstance().GetCurrentState().Debugging())
        return;

    Vec2 center( box.GetPosition() );
    SDL_Point points[5];

    Vec2 point = (Vec2(box.x, box.y) - center).Rotate( associated.angleDeg/(180/PI) )
                    + center - Camera::pos;
    points[0] = {(int)point.x, (int)point.y};
    points[4] = {(int)point.x, (int)point.y};
    
    point = (Vec2(box.x + box.w, box.y) - center).Rotate( associated.angleDeg/(180/PI) )
                    + center - Camera::pos;
    points[1] = {(int)point.x, (int)point.y};
    
    point = (Vec2(box.x + box.w, box.y + box.h) - center).Rotate( associated.angleDeg/(180/PI) )
                    + center - Camera::pos;
    points[2] = {(int)point.x, (int)point.y};
    
    point = (Vec2(box.x, box.y + box.h) - center).Rotate( associated.angleDeg/(180/PI) )
                    + center - Camera::pos;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
}

bool Collider::Is (std::string type) {
    return (type == "Collider");
}

bool Collider::Is (GameObjID type) {
    return (type & this->type);
}