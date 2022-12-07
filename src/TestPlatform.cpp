#include "GentooEngine.h"
#include "TestObjects.h"

Platform::Platform (
    GameObject& associated, Direction direction, float positionLimit, bool body
): Component(associated) {
    this->direction = direction;
    this->positionLimit = positionLimit;
    bodied = body;

    relativeOffset = 0.0f;
    movementIsOver = false;
    activeRect = MOVE;
}

Platform::~Platform () {
    Camera::RemoveMethod(this);
}

void Platform::Start () {
    if (not (bodied and associated.GetComponent("Collider")))
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
            currentDisplacement = displacement.y * 2.0 * dt;
            relativeOffset += currentDisplacement;
            currentPosition = Camera::pos.y + relativeOffset;
            if (currentPosition > positionLimit) {
                Camera::masterOffset.y += currentDisplacement;
            } else {
                relativeOffset -= positionLimit - currentPosition;
                Camera::masterOffset.y += currentDisplacement - (positionLimit - currentPosition);
                movementIsOver = true;
                SDL_Log("pos %f", Camera::pos.y);
                SDL_Log("rel %f", relativeOffset);
                SDL_Log("mas %f", Camera::masterOffset.y);
            } break;
        default: break;
    }
    if (movementIsOver)
        LimitCamera();
}

void* Platform::UndoCameraMovement () {
    float dt = Game::GetInstance().GetCurrentState().stateDt;
    float currentDisplacement;
    switch (direction) {
        case DOWN:
            currentDisplacement = displacement.y * 2.0 * dt;
            relativeOffset += currentDisplacement;
            if (relativeOffset < 0.0f) {
                Camera::masterOffset.y += currentDisplacement;
            } else {
                relativeOffset -= currentDisplacement;
                Camera::masterOffset.y -= relativeOffset;
                SDL_Log("pos %f", Camera::pos.y);
                SDL_Log("rel %f", relativeOffset);
                SDL_Log("mas %f", Camera::masterOffset.y);
                relativeOffset = 0.0f;
                movementIsOver = false;
            } break;
        default: break;
    }
    if (not movementIsOver)
        Camera::RemoveMethod(this);
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