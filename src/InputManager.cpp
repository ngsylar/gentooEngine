#include "GentooEngine.h"

InputManager& InputManager::GetInstance () {
    static InputManager inputManager;
    return inputManager;
}

InputManager::InputManager () {
    std::fill_n(mouseState, 6, false);
    std::fill_n(mouseUpdate, 6, 0);
    mouseX = 0;
    mouseY = 0;
    updateCounter = 0;
    quitRequested = false;
}

InputManager::~InputManager () {}

void InputManager::Update () {
    SDL_Event event;

    SDL_GetMouseState(&mouseX, &mouseY);
    quitRequested = false;
    updateCounter++;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quitRequested = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouseState[event.button.button] = true;
                mouseUpdate[event.button.button] = updateCounter;
                break;
            case SDL_MOUSEBUTTONUP:
                mouseState[event.button.button] = false;
                mouseUpdate[event.button.button] = updateCounter;
                break;
            case SDL_KEYDOWN:
                if (event.key.repeat == 0) {
                    keyState[event.key.keysym.sym] = true;
                    keyUpdate[event.key.keysym.sym] = updateCounter;
                } break;
            case SDL_KEYUP:
                keyState[event.key.keysym.sym] = false;
                keyUpdate[event.key.keysym.sym] = updateCounter;
                break;
            default: break;
        }
    }
}

bool InputManager::KeyPress (int key) {
    return (keyState[key] and (keyUpdate[key] == updateCounter));
}

bool InputManager::KeyRelease (int key) {
    return (!keyState[key] and (keyUpdate[key] == updateCounter));
}

bool InputManager::IsKeyDown (int key) {
    return keyState[key];
}

bool InputManager::MousePress (int button) {
    return (mouseState[button] and (mouseUpdate[button] == updateCounter));
}

bool InputManager::MouseRelease (int button) {
    return (!mouseState[button] and (mouseUpdate[button] == updateCounter));
}

bool InputManager::IsMouseDown (int button) {
    return mouseState[button];
}

Vec2 InputManager::GetMousePosition () {
    return Vec2(mouseX+Camera::pos.x, mouseY+Camera::pos.y);
}

int InputManager::GetMouseX () {
    return mouseX;
}

int InputManager::GetMouseY () {
    return mouseY;
}

bool InputManager::QuitRequested () {
    return quitRequested;
}