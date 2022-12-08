#include "GentooEngine.h"
#include "TestObjects.h"

Platform::Platform (
    GameObject& associated, Direction direction, float positionLimit
): Component(associated) {
    this->direction = direction;
    this->positionLimit = positionLimit;
    bodied = false;

    relativeOffset = 0.0f;
    movementIsOver = false;
    activeRect = MOVE;
}

Platform::~Platform () {
    Camera::RemoveMethod(this);
}

void Platform::SetTrigger (Rect rectToMove, Rect rectToUndo) {
    Collider* collider = (Collider*)associated.GetComponent("Collider");
    colliderRects = {rectToMove, rectToUndo};
    associated.box = rectToMove;
    if (collider == nullptr)
        collider = new Collider(associated);
    collider->isTrigger = true;
    associated.AddComponent(collider);
    bodied = true;
}

void Platform::Start () {
    if (not bodied)
        Camera::AddMethod(this, std::bind(&LimitCamera, this));
}

void Platform::NotifyCollision (GameObject& other) {
    if ((not bodied) or (other.label != "Player"))
        return;

    if (activeRect == MOVE) {
        displacement = -(other.box.GetPosition() - positionLimit);
        associated.box = colliderRects[UNDO];
        Camera::RemoveMethod(this);
        Camera::AddMethod(this, std::bind(&MoveCamera, this));
        activeRect = UNDO;
    } else {
        displacement = other.box.GetPosition() - positionLimit;
        associated.box = colliderRects[MOVE];
        Camera::RemoveMethod(this);
        Camera::masterOffset.y += relativeOffset; // editar: so considera o eixo Y
        Camera::AddMethod(this, std::bind(&UndoCameraMovement, this));
        activeRect = MOVE;
    }
}

void* Platform::MoveCamera () {
    if (movementIsOver) {
        LimitCamera();
        return nullptr;
    }
    float dt = Game::GetInstance().GetCurrentState().stateDt;
    float currentDisplacement, currentPosition;
    switch (direction) {
        case DOWN:
            currentDisplacement = displacement.y * 1.5f * dt;
            relativeOffset += currentDisplacement;
            currentPosition = Camera::pos.y + relativeOffset;
            if (currentPosition > positionLimit) {
                Camera::masterOffset.y += currentDisplacement;
            } else {
                Camera::masterOffset.y -= relativeOffset - currentDisplacement;
                relativeOffset -= currentPosition - positionLimit;
                movementIsOver = true;
                // SDL_Log("pos %f", Camera::pos.y);
                // SDL_Log("rel %f", relativeOffset);
                // SDL_Log("mas %f", Camera::masterOffset.y);
            } break;
        default: break;
    }
    if (movementIsOver)
        LimitCamera();
    return nullptr;
}

void* Platform::UndoCameraMovement () {
    float dt = Game::GetInstance().GetCurrentState().stateDt;
    float currentDisplacement;
    switch (direction) {
        case DOWN:
            currentDisplacement = displacement.y * 1.5f * dt;
            relativeOffset += currentDisplacement;
            if (relativeOffset < 0.0f) {
                Camera::masterOffset.y += currentDisplacement;
            } else {
                Camera::masterOffset.y += currentDisplacement - relativeOffset;
                relativeOffset = 0.0f;
                // SDL_Log("pos %f", Camera::pos.y);
                // SDL_Log("rel %f", relativeOffset);
                // SDL_Log("mas %f", Camera::masterOffset.y);
                movementIsOver = false;
            } break;
        default: break;
    }
    if (not movementIsOver)
        Camera::RemoveMethod(this);
    return nullptr;
}

void* Platform::LimitCamera () {
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