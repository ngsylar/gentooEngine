#include "GentooEngine.h"

InputManager& InputManager::GetInstance () {
    static InputManager inputManager;
    return inputManager;
}

InputManager::InputManager () {
    std::fill_n(mouseState, 6, false);
    std::fill_n(mouseUpdate, 6, 0);

    std::fill_n(controllerState, 21, false);
    std::fill_n(controllerUpdate, 21, 0);

    mouseX = 0;
    mouseY = 0;
    updateCounter = 0;
    quitRequested = false;

    //TODO controller connection/disconnection handling
    controller = nullptr;
    if(SDL_NumJoysticks()<1) {
        SDL_Log("InputManager: No avaliable controller.");
    } else {
    controller = SDL_GameControllerOpen(0);
        if(controller == nullptr) {
            SDL_Log("InputManager: Unable to open controller. SDL Error: %s\n", SDL_GetError());
        }
    }
    SDL_GameControllerGetType(controller);
}

InputManager::~InputManager () {
    if(controller != nullptr) {
        SDL_GameControllerClose(controller);
        controller = nullptr;
    }
}

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
            case SDL_CONTROLLERAXISMOTION: 
                if(std::abs(event.caxis.value)>DEADZONE) {
                    axis[event.caxis.axis] = (event.caxis.value < 0 ? -1 : 1);
                } else {
                    axis[event.caxis.axis] = 0;
                }
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                controllerState[event.cbutton.button] = true;
                controllerUpdate[event.cbutton.button] = updateCounter;
                break;
            
            case SDL_CONTROLLERBUTTONUP:
                controllerState[event.cbutton.button] = false;
                controllerUpdate[event.cbutton.button] = updateCounter;

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

bool InputManager::ControllerPress (int button) {
    return (controllerState[button] and (controllerUpdate[button] == updateCounter));
}

bool InputManager::ControllerRelease (int button) {
    return (!controllerState[button] and (controllerUpdate[button] == updateCounter));
}

bool InputManager::IsControllerDown (int button) {
    return controllerState[button];
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

int InputManager::GetAxisMotion(int index){
    return axis[index];
}