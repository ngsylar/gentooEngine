#include "GentooEngine.h"

GameObject* Camera::focus = nullptr;
bool Camera::freeCamera = false;
Vec2 Camera::pos, Camera::speed;

void Camera::Follow (GameObject* newFocus) {
    focus = newFocus;
}

void Camera::Unfollow () {
    focus = nullptr;
}

void Camera::EnableFree () {
    freeCamera = true;
}

void Camera::DisableFree () {
    freeCamera = false;
}

void Camera::Update (float dt) {
    InputManager& input = InputManager::GetInstance();

    if (focus) {
        pos = focus->box.GetPosition() - (Game::GetInstance().GetWindowSize() * 0.5f);
    }
    else if (freeCamera) {
        if (input.IsKeyDown(KEY_W)) {
            speed.y = -1;
        }
        if (input.IsKeyDown(KEY_S)) {
            speed.y = 1;
        }
        if (input.IsKeyDown(KEY_A)) {
            speed.x = -1;
        }
        if (input.IsKeyDown(KEY_D)) {
            speed.x = 1;
        }
        if (speed.x and speed.y) {
            speed.Normalize();
        }
        pos += speed * CAMERA_SPEED * dt;
    }

    speed.x = 0;
    speed.y = 0;
}

void Camera::Reset () {
    focus = nullptr;
    pos = Vec2();
    speed = Vec2();
}

Vec2 Camera::GetPosition () {
    return (pos + (Game::GetInstance().GetWindowSize() * 0.5f));
}