#include "GentooEngine.h"

GameObject* Camera::focus = nullptr;
Vec2 Camera::pos, Camera::speed, Camera::deadZoneLength;
Vec2 Camera::focusPreviousPosition, Camera::focusLastDirection;
float Camera::acceleration = 0.0f; // editar
bool Camera::freeCamera = false;

void Camera::Follow (GameObject* newFocus) {
    focus = newFocus;
    focusPreviousPosition = focus->box.GetPosition();
    focusLastDirection = Vec2();
    speed = Vec2();

    deadZoneLength = Vec2(50,0); // remover
}

void Camera::Unfollow () {
    focus = nullptr;
}

void Camera::Cinemachine (float dt) {
    if (deadZoneLength == Vec2())
        return;

    Vec2 focusPosition = focus->box.GetPosition();
    Vec2 focusDirection = focusPosition - focusPreviousPosition;
    focusPreviousPosition = focus->box.GetPosition();

    if (focusDirection.x < 0) {
        if (speed.x > 0.08f)
            speed.x -= dt * 2.0f;
        else if (speed.x < -0.08f)
            speed.x += dt * 2.0f;
        else speed.x = 0;
        focusLastDirection.x = LEFT;
    }
    else if (focusDirection.x > 0) {
        if (speed.x < -0.08f)
            speed.x += dt * 2.0f;
        else if (speed.x > 0.08f)
            speed.x -= dt * 2.0f;
        else speed.x = 0;
        focusLastDirection.x = RIGHT;
    }
    else if (focusLastDirection.x == LEFT) {
        if (speed.x > -1)
            speed.x -= dt * 2.0f;
        else speed.x = -1;
    }
    else if (focusLastDirection.x == RIGHT) {
        if (speed.x < 1)
            speed.x += dt * 2.0f;
        else speed.x = 1;
    }
}

void Camera::Update (float dt) {
    InputManager& input = InputManager::GetInstance();

    if (focus) {
        Cinemachine(dt);

        Vec2 cameraPosition = (Game::GetInstance().GetWindowSize() * 0.5f);
        pos = focus->box.GetPosition() + (deadZoneLength * speed) - cameraPosition;
    }
    else if (freeCamera) {
        if (input.IsKeyDown(KEY_W))
            speed.y = -1;
        if (input.IsKeyDown(KEY_S))
            speed.y = 1;
        if (input.IsKeyDown(KEY_A))
            speed.x = -1;
        if (input.IsKeyDown(KEY_D))
            speed.x = 1;

        if (speed.x and speed.y)
            speed.Normalize();
        pos += speed * CAMERA_SPEED * dt;

        speed.x = 0;
        speed.y = 0;
    }
}

void Camera::Reset () {
    focus = nullptr;
    pos = Vec2();
    speed = Vec2();
}

Vec2 Camera::GetPosition () {
    return (pos + (Game::GetInstance().GetWindowSize() * 0.5f));
}