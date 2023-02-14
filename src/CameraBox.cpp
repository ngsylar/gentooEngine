#include "GentooEngine.h"

CameraBox::CameraBox (
    GameObject& associated, GameObject* focus, float spacingX, float spacingY
): Component(associated) {
    this->focus = Game::GetInstance().GetCurrentState().GetObjectPtr(focus);
    associated.box.SetSize(fabs(spacingX), fabs(spacingY));

    contacts = {NONE, NONE};
    this->isDynamic = false;
    dynamicFactor = Vec2(1.0f, 1.0f);

    type = ComponentType::_CameraBox;
}

void CameraBox::Awaken () {
    focusBox.x = focus.lock()->box.x + focusBoxOffset.x;
    focusBox.y = focus.lock()->box.y + focusBoxOffset.y;
    focusBox.w = (focusBoxOffset.w > 0.0f) ? focusBoxOffset.w : focus.lock()->box.w;
    focusBox.h = (focusBoxOffset.h > 0.0f) ? focusBoxOffset.h : focus.lock()->box.h;
    associated.box.w += focusBox.w; associated.box.h += focusBox.h;
    associated.box.SetPosition(focusBox.GetPosition());
}

void CameraBox::Follow (GameObject* focus, float spacingX, float spacingY) {
    this->focus = Game::GetInstance().GetCurrentState().GetObjectPtr(focus);
    focusBox.x = focus->box.x + focusBoxOffset.x;
    focusBox.y = focus->box.y + focusBoxOffset.y;
    focusBox.w = (focusBoxOffset.w > 0.0f) ? focusBoxOffset.w : focus->box.w;
    focusBox.h = (focusBoxOffset.h > 0.0f) ? focusBoxOffset.h : focus->box.h;
    associated.box.SetSize(focusBox.w+fabs(spacingX), focusBox.h+fabs(spacingY));
    associated.box.SetPosition(focusBox.GetPosition());
}

void CameraBox::Update (float dt) {
    if (focus.expired()) {
        associated.RequestDelete();
        return;
    }

    focusBox.x = (int)focus.lock()->box.x+focusBoxOffset.x;
    focusBox.y = (int)focus.lock()->box.y+focusBoxOffset.y;
    std::pair<ContactSide, float> contact;

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
        contact = DetectContact(focusBox.x, focusBox.w, associated.box.x, associated.box.w);
        if (contact.first != NONE)
            associated.box.x = contact.second;
        contacts[X] = contact.first;

        contact = DetectContact(focusBox.y, focusBox.h, associated.box.y, associated.box.h);
        if (contact.first != NONE)
            associated.box.y = contact.second;
        contacts[Y] = contact.first;
    }

    for (int i=0; i < (int)foreignMethods.size(); i++)
        foreignMethods[i].second();
}

std::pair<CameraBox::ContactSide, float> CameraBox::DetectContact (
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
    std::pair<bool, float> contact, contactD;
    float displacement;

    contact = DetectContact(focusPoint, focusSize, selfPoint, selfSize);
    contactD = DetectContact(focusPoint, focusSize, pointDR, selfSize);
    displacement = contact.second - selfPoint;
    
    if (contact.first) {
        if ((displacement < 0.0f) and (lastDisp < 0.0f)) {
            float pointDA = (contactD.first) ? contactD.second : pointDR;
            destPoint = (lastDisp < displacement) ? pointDA : contact.second;
        } else if ((displacement > 0.0f) and (lastDisp > 0.0f)) {
            float pointDA = (contactD.first) ? contactD.second : pointDR;
            destPoint = (lastDisp > displacement) ? pointDA : contact.second;
        } else destPoint = (contactD.first) ? contact.second : pointDR;
    } else destPoint = (contactD.first) ? contactD.second : pointDR;
}

CameraBox::ContactSide CameraBox::GetContact (CameraBox::Axis axis) {
    return contacts[axis];
}

// DEBUG
void CameraBox::Render () {
    if (not Game::GetInstance().GetCurrentState().Debugging()) return;

    Rect box( associated.box );
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

bool CameraBox::Is (ComponentType type) {
    return (type & this->type);
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