#include "GentooEngine.h"
#include "TestObjects.h"

Platform::Platform (GameObject& associated): Component(associated) {
    direction = NONE;
    positionLimit = 0.0f;
    bodied = false;
    isActive = false;
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
    if (bodied)
        Camera::AddMethod(this, std::bind(&HandleCameraWithBody, this));
    else Camera::AddMethod(this, std::bind(&HandleCamera, this));
}

void Platform::SetBody (bool body) {
    Camera::RemoveMethod(this);
    bodied = body;
    if (bodied)
        Camera::AddMethod(this, std::bind(&HandleCameraWithBody, this));
    else Camera::AddMethod(this, std::bind(&HandleCamera, this));
}

void Platform::NotifyCollision (GameObject& other) {
    if (not (bodied and (not isActive) and (other.label == "Player")))
        return;
    displacement = -(Camera::pos - positionLimit);
    isActive = true;
}

void Platform::NotifyNoCollision (GameObject& other) {
    if (bodied and isActive and (other.label == "Player"))
        isActive = false;
}

void* Platform::HandleCameraWithBody () {
    // SDL_Log("camera: %f", Camera::pos.y);
    // SDL_Log("offset: %f", Camera::offset.y);
    if (not isActive) {
        
        return nullptr;
    }

    float dt = Game::GetInstance().GetCurrentState().stateDt;
    switch (direction) {
        case LEFT:  if ((Camera::pos.x + Camera::masterOffset.x) < positionLimit)
                        Camera::masterOffset.x += displacement.x * 2.0 * dt;
                    else Camera::masterOffset.x = positionLimit;
                    break;
        case RIGHT: if ((Camera::pos.x + Camera::masterOffset.x) > positionLimit)
                        Camera::masterOffset.x += displacement.x * 2.0 * dt;
                    else Camera::masterOffset.x = positionLimit; 
                    break;
        case UP:    if ((Camera::pos.y + Camera::masterOffset.y) < positionLimit)
                        Camera::masterOffset.y += displacement.y * 2.0 * dt;
                    else Camera::masterOffset.y = positionLimit;
                    break;
        case DOWN:  if ((Camera::pos.y + Camera::masterOffset.y) > positionLimit)
                        Camera::masterOffset.y += displacement.y * 2.0 * dt;
                    else Camera::masterOffset.y = positionLimit;
                    break;
        default: break;
    }
    return nullptr;
}

void* Platform::HandleCamera () {
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