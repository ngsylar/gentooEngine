#include "GentooEngine.h"
#include "TestObjects.h"

Platform::Platform (GameObject& associated): Component(associated) {
    direction = NONE;
    positionLimit = 0.0f;
    bodied = false;
}

Platform::Platform (
    GameObject& associated, Direction direction, float positionLimit, bool bodied
): Component(associated) {
    this->direction = direction;
    this->positionLimit = positionLimit;
    this->bodied = bodied;
}

Platform::~Platform () {
    Camera::RemoveMethod(this);
}

void Platform::Start () {
    Camera::AddMethod(this, std::bind(&HandleCamera, this));
}

void* Platform::HandleCamera () {
    // SDL_Log("camera: %f", Camera::pos.y);
    // SDL_Log("offset: %f", Camera::offset.y);

    switch (direction) {
        case LEFT:  if (Camera::pos.x < positionLimit)
                    Camera::pos.y = positionLimit;
                    break;
        case RIGHT: if (Camera::pos.x > positionLimit)
                    Camera::pos.y = positionLimit;
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