#include "GentooEngine.h"

#define CINEMACHINE_LENGTH          14.0f, 86.0f
#define CINEMACHINE_SLICES          8, 32, 2, 24
#define CINEMACHINE_OFFSET          0.0f, 12.0f
#define CINEMACHINE_SETUP           true, true, true, false, true, true, false, false

CameraSquare::CameraSquare (
    GameObject& associated, GameObject* focus, float spacingX, float spacingY
): Component(associated) {
    this->focus = Game::GetInstance().GetCurrentState().GetObjectPtr(focus);
    associated.box.w = fabs(spacingX);
    associated.box.h = fabs(spacingY);
}

void CameraSquare::Follow (GameObject* focus, float spacingX, float spacingY) {
    this->focus = Game::GetInstance().GetCurrentState().GetObjectPtr(focus);
    Collider* focusCollider = (Collider*)focus->GetComponent("Collider");
    Rect focusBox = focusCollider ? focusCollider->box : focus->box;
    associated.box.w = focusBox.w + fabs(spacingX);
    associated.box.h = focusBox.h + fabs(spacingY);
    associated.box.SetPosition(focusBox.GetPosition());
}

void CameraSquare::Start () {
    Collider* focusCollider = (Collider*)focus.lock()->GetComponent("Collider");
    Rect focusBox = focusCollider ? focusCollider->box : focus.lock()->box;
    associated.box.w += focusBox.w;
    associated.box.h += focusBox.h;
    associated.box.SetPosition(focusBox.GetPosition());

    Camera::Follow(
        &associated, Vec2(CINEMACHINE_LENGTH), CINEMACHINE_SLICES,
        Camera::RIGHT, Camera::UP, Vec2(CINEMACHINE_OFFSET));
    Camera::cinemachine.Setup(CINEMACHINE_SETUP);
    Camera::offset.y = 0.0f;
}

void CameraSquare::Update (float dt) {
    if (focus.expired()) return;
    
    Collider* focusCollider = (Collider*)focus.lock()->GetComponent("Collider");
    Rect focusBox = focusCollider ? focusCollider->box : focus.lock()->box;
    
    if (focusBox.x < associated.box.x)
        associated.box.x = focusBox.x;
    else if (focusBox.x+focusBox.w > associated.box.x+associated.box.w)
        associated.box.x = focusBox.x - (associated.box.w - focusBox.w);
    if (focusBox.y < associated.box.y)
        associated.box.y = focusBox.y;
    else if (focusBox.y+focusBox.h > associated.box.y+associated.box.h)
        associated.box.y = focusBox.y - (associated.box.h - focusBox.h);
}

// DEBUG
void CameraSquare::Render () {
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