#include "GentooEngine.h"
#include "PlatformCamera.h"

PlatformCamera::PlatformCamera (
    GameObject& associated, Direction direction, float positionLimit
): Component(associated) {
    this->direction = direction;
    this->positionLimit = positionLimit;
    activeRect = MOVE;
    bodied = false;
    isMoved = false;
    type = GameObjID::_PlatformCamera;
}

PlatformCamera::~PlatformCamera () {
    Camera::RemoveMethod(this);
}

void PlatformCamera::SetTrigger (Rect rectToMove, Rect rectToUndo) {
    Collider* collider = (Collider*)associated.GetComponent(GameObjID::_Collider);
    colliderRects = {rectToMove, rectToUndo};
    associated.box = rectToMove;
    if (collider == nullptr)
        collider = new Collider(associated);
    collider->isTrigger = true;
    associated.AddComponent(collider);
    bodied = true;
}

void PlatformCamera::Start () {
    if (not bodied)
        Camera::AddMethod(this, std::bind(&LimitCamera, this));
}

void PlatformCamera::NotifyCollision (GameObject& other) {
    if ((not bodied) or (other.label != "Player"))
        return;

    if (activeRect == MOVE) {
        targetDisplacement = -(other.box.GetPosition() - positionLimit);
        associated.box = colliderRects[UNDO];
        Camera::RemoveMethod(this);
        Camera::AddMethod(this, std::bind(&MoveCamera, this));
        activeRect = UNDO;
    } else {
        targetDisplacement = other.box.GetPosition() - positionLimit;
        associated.box = colliderRects[MOVE];
        Camera::RemoveMethod(this);
        Camera::AddMethod(this, std::bind(&UndoCameraMovement, this));
        Camera::masterOffset += relativeOffset;
        activeRect = MOVE;
    }
}

void* PlatformCamera::MoveCamera () {
    if (isMoved) {
        LimitCamera();
        return nullptr;
    }
    float dt = Game::GetInstance().GetCurrentState().stateDt;
    float currentDisplacement, currentPosition;
    switch (direction) {
        case DOWN:
            currentDisplacement = targetDisplacement.y * 1.5f * dt;
            relativeOffset.y += currentDisplacement;
            currentPosition = Camera::pos.y + relativeOffset.y;
            if (currentPosition > positionLimit) {
                Camera::masterOffset.y += currentDisplacement;
            } else {
                Camera::masterOffset.y -= relativeOffset.y - currentDisplacement;
                relativeOffset.y -= currentPosition - positionLimit;
                isMoved = true;
                // SDL_Log("pos %f", Camera::pos.y);
                // SDL_Log("rel %f", relativeOffset);
                // SDL_Log("mas %f", Camera::masterOffset.y);
            } break;
        default: break;
    }
    if (isMoved)
        LimitCamera();
    return nullptr;
}

void* PlatformCamera::UndoCameraMovement () {
    float dt = Game::GetInstance().GetCurrentState().stateDt;
    float currentDisplacement;
    switch (direction) {
        case DOWN:
            currentDisplacement = targetDisplacement.y * 1.5f * dt;
            relativeOffset.y += currentDisplacement;
            if (relativeOffset.y < 0.0f) {
                Camera::masterOffset.y += currentDisplacement;
            } else {
                Camera::masterOffset.y += currentDisplacement - relativeOffset.y;
                relativeOffset.y = 0.0f;
                // SDL_Log("pos %f", Camera::pos.y);
                // SDL_Log("rel %f", relativeOffset);
                // SDL_Log("mas %f", Camera::masterOffset.y);
                isMoved = false;
            } break;
        default: break;
    }
    if (not isMoved)
        Camera::RemoveMethod(this);
    return nullptr;
}

void* PlatformCamera::LimitCamera () {
    // SDL_Log("camera: %f", Camera::pos.y);
    // SDL_Log("offset: %f", Camera::offset.y);

    switch (direction) {
        case LEFT:  if (Camera::pos.x < positionLimit)
                    Camera::pos.x = positionLimit;
                    break;
        case RIGHT: if (Camera::pos.x > positionLimit)
                    Camera::pos.x = positionLimit;
                    break;
        case UP:    if (Camera::pos.y < positionLimit)
                    Camera::pos.y = positionLimit;
                    break;
        case DOWN:  if (Camera::pos.y > positionLimit)
                    Camera::pos.y = positionLimit;
                    break;
        default: break;
    }
    return nullptr;
}

bool PlatformCamera::Is (GameObjID type) {
    return (type & this->type);
}