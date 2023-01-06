#include "GentooEngine.h"

CameraBox::CameraBox (
    GameObject& associated, GameObject* focus, float spacingX, float spacingY
): Component(associated) {
    this->focus = Game::GetInstance().GetCurrentState().GetObjectPtr(focus);
    associated.box.SetSize(fabs(spacingX), fabs(spacingY));

    contacts = {NONE, NONE};
    this->isDynamic = false;
    dynamicFactor = Vec2(1.0f, 1.0f);
}

void CameraBox::Follow (GameObject* focus, float spacingX, float spacingY) {
    this->focus = Game::GetInstance().GetCurrentState().GetObjectPtr(focus);

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
}

void CameraBox::Update (float dt) {
    if (focus.expired()) {
        associated.RequestDelete();
        return;
    }

    Collider* focusCollider = (Collider*)focus.lock()->GetComponent("Collider");
    Rect focusBox = focusCollider ? focusCollider->box : focus.lock()->box;
    std::pair<ContactSide, float> motion;

    if (isDynamic) {
        Vec2 position = Vec2(associated.box.x, associated.box.y);
        Vec2 positionDR = position + (lastDisplacement * dynamicFactor);

        MoveDynamically(
            focusBox.x, focusBox.w, associated.box.w, position.x,
            positionDR.x, lastDisplacement.x, associated.box.x);
        MoveDynamically(
            focusBox.y, focusBox.h, associated.box.h, position.y,
            positionDR.y, lastDisplacement.y, associated.box.y);

        lastDisplacement = (Vec2(associated.box.x, associated.box.y) - position);
    }
    else {
        motion = DetectMotion(focusBox.x, focusBox.w, associated.box.x, associated.box.w);
        if (motion.first != NONE)
            associated.box.x = motion.second;
        contacts[X] = motion.first;

        motion = DetectMotion(focusBox.y, focusBox.h, associated.box.y, associated.box.h);
        if (motion.first != NONE)
            associated.box.y = motion.second;
        contacts[Y] = motion.first;
    }

    for (int i=0; i < (int)foreignMethods.size(); i++)
        foreignMethods[i].second();
}

std::pair<CameraBox::ContactSide, float> CameraBox::DetectMotion (
    float focusPoint, float focusSize, float selfPoint, float selfSize
) {
    if (focusPoint < selfPoint)
        return std::make_pair(NEGATIVE, focusPoint);
    else if ((focusPoint + focusSize) > (selfPoint + selfSize))
        return std::make_pair(POSITIVE, focusPoint-(selfSize-focusSize));
    else return std::make_pair(NONE, 0.0f);
}

// editar: nao esta salvando os pontos de contato 
void CameraBox::MoveDynamically (
    float& focusPoint, float& focusSize, float& selfSize,
    float& selfPoint, float& pointDR, float& lastDisp, float& destPoint
) {
    std::pair<bool, float> motion, motionD;
    float displacement;

    motion = DetectMotion(focusPoint, focusSize, selfPoint, selfSize);
    motionD = DetectMotion(focusPoint, focusSize, pointDR, selfSize);
    displacement = motion.second - selfPoint;
    
    if (motion.first) {
        if ((displacement < 0.0f) and (lastDisp < 0.0f)) {
            float pointDA = (motionD.first) ? motionD.second : pointDR;
            destPoint = (lastDisp < displacement) ? pointDA : motion.second;
        } else if ((displacement > 0.0f) and (lastDisp > 0.0f)) {
            float pointDA = (motionD.first) ? motionD.second : pointDR;
            destPoint = (lastDisp > displacement) ? pointDA : motion.second;
        } else destPoint = (motionD.first) ? motion.second : pointDR;
    } else destPoint = (motionD.first) ? motionD.second : pointDR;
}

// DEBUG
void CameraBox::Render () {
    if (not Game::GetInstance().GetCurrentState().Debugging()) return;

    Rect box( associated.box );
    Vec2 center( box.GetPosition() );
    SDL_Point points[5];

    Vec2 point = Vec2(box.x, box.y) - Camera::pos;
    points[0] = {(int)point.x, (int)point.y};
    points[4] = {(int)point.x, (int)point.y};

    point = Vec2(box.x + box.w, box.y) - Camera::pos;
    points[1] = {(int)point.x, (int)point.y};
    
    point = Vec2(box.x + box.w, box.y + box.h) - Camera::pos;
    points[2] = {(int)point.x, (int)point.y};
    
    point = Vec2(box.x, box.y + box.h) - Camera::pos;
    points[3] = {(int)point.x, (int)point.y};

    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
}

bool CameraBox::Is (std::string type) {
    return (type == "CameraBox");
}

void CameraBox::AddMethod(Component* component, std::function<void*()> method) {
    foreignMethods.push_back(std::make_pair(component, method));
}

void CameraBox::RemoveMethod(Component* component) {
    for (int i=(int)foreignMethods.size()-1; i >= 0; i--)
        if (foreignMethods[i].first == component)
            foreignMethods.erase(foreignMethods.begin()+i);
}

void CameraBox::ClearMethods () {
    foreignMethods.clear();
}

CameraBox::~CameraBox () {
    foreignMethods.clear();
}