#include "GentooEngine.h"

#define CINEMACHINE_LENGTH          14.0f, 86.0f
#define CINEMACHINE_SLICES          8, 32, 2, 24
#define CINEMACHINE_OFFSET          0.0f, 12.0f
#define CINEMACHINE_SETUP           true, true, true, false, true, true, false, false

CameraBox::CameraBox (
    GameObject& associated, GameObject* focus, float spacingX, float spacingY, bool isDynamic
): Component(associated) {
    this->focus = Game::GetInstance().GetCurrentState().GetObjectPtr(focus);
    this->isDynamic = isDynamic;
    associated.box.SetSize(fabs(spacingX), fabs(spacingY));
}

void CameraBox::Follow (GameObject* focus, float spacingX, float spacingY, bool isDynamic) {
    this->focus = Game::GetInstance().GetCurrentState().GetObjectPtr(focus);
    this->isDynamic = isDynamic;

    Collider* focusCollider = (Collider*)focus->GetComponent("Collider");
    Rect focusBox = focusCollider ? focusCollider->box : focus->box;
    associated.box.SetSize(focusBox.w+fabs(spacingX), focusBox.h+fabs(spacingY));
    associated.box.SetPosition(focusBox.GetPosition());
}

void CameraBox::Start () {
    Collider* focusCollider = (Collider*)focus.lock()->GetComponent("Collider");
    Rect focusBox = focusCollider ? focusCollider->box : focus.lock()->box;
    associated.box.w += focusBox.w; associated.box.h += focusBox.h;
    associated.box.SetPosition(focusBox.GetPosition());

    Camera::Follow(
        &associated, Vec2(CINEMACHINE_LENGTH), CINEMACHINE_SLICES,
        Camera::RIGHT, Camera::UP, Vec2(CINEMACHINE_OFFSET));
    Camera::cinemachine.Setup(CINEMACHINE_SETUP);
    Camera::offset.y = 0.0f;
}

void CameraBox::Update (float dt) {
    if (focus.expired()) return;

    Collider* focusCollider = (Collider*)focus.lock()->GetComponent("Collider");
    Rect focusBox = focusCollider ? focusCollider->box : focus.lock()->box;
    std::pair<bool, float> collision;    

    if (isDynamic) {
        std::pair<bool, float> collDyn;
        Vec2 position = Vec2(associated.box.x, associated.box.y);
        Vec2 posDyn = position + lastDisplacement;
        float displacement;

        collision = DetectCollision(focusBox.x, focusBox.w, position.x, associated.box.w);
        collDyn = DetectCollision(focusBox.x, focusBox.w, posDyn.x, associated.box.w);
        displacement = collision.second - position.x;
        
        if (collision.first) {
            if ((displacement < 0.0f) and (lastDisplacement.x < 0.0f)) {
                float collDynPos = (collDyn.first) ? collDyn.second : posDyn.x;
                associated.box.x = (lastDisplacement.x < displacement) ? collDynPos : collision.second;
            } else if ((displacement > 0.0f) and (lastDisplacement.x > 0.0f)) {
                float collDynPos = (collDyn.first) ? collDyn.second : posDyn.x;
                associated.box.x = (lastDisplacement.x > displacement) ? collDynPos : collision.second;
            } else if (not collDyn.first) {
                associated.box.x = posDyn.x;
            } else {
                associated.box.x = collision.second;
            }
        } else if (collDyn.first)
            associated.box.x = collDyn.second;
        else associated.box.x = posDyn.x;

        collision = DetectCollision(focusBox.y, focusBox.h, position.y, associated.box.h);
        collDyn = DetectCollision(focusBox.y, focusBox.h, posDyn.y, associated.box.h);
        displacement = collision.second - position.y;
        
        if (collision.first) {
            if ((displacement < 0.0f) and (lastDisplacement.y < 0.0f)) {
                float collDynPos = (collDyn.first) ? collDyn.second : posDyn.y;
                associated.box.y = (lastDisplacement.y < displacement) ? collDynPos : collision.second;
            } else if ((displacement > 0.0f) and (lastDisplacement.y > 0.0f)) {
                float collDynPos = (collDyn.first) ? collDyn.second : posDyn.y;
                associated.box.y = (lastDisplacement.y > displacement) ? collDynPos : collision.second;
            } else if (not collDyn.first) {
                associated.box.y = posDyn.y;
            } else {
                associated.box.y = collision.second;
            }
        } else if (collDyn.first)
            associated.box.y = collDyn.second;
        else associated.box.y = posDyn.y;

        lastDisplacement = Vec2(associated.box.x, associated.box.y) - position;
    } else {
        collision = DetectCollision(focusBox.x, focusBox.w, associated.box.x, associated.box.w);
        if (collision.first) associated.box.x = collision.second;
        collision = DetectCollision(focusBox.y, focusBox.h, associated.box.y, associated.box.h);
        if (collision.first) associated.box.y = collision.second;
    }
}

std::pair<bool, float> CameraBox::DetectCollision (
    float focusPoint, float focusSize, float selfPoint, float selfSize
) {
    if (focusPoint < selfPoint)
        return std::make_pair(true, focusPoint);
    else if ((focusPoint + focusSize) > (selfPoint + selfSize))
        return std::make_pair(true, focusPoint-(selfSize-focusSize));
    else
        return std::make_pair(false, 0.0f);
}

// DEBUG
void CameraBox::Render () {
    if (not Game::GetInstance().GetCurrentState().Debugging())
        return;

    Rect box( associated.box );
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

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
}