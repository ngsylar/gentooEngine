#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <unordered_map>

#define INCLUDE_SDL
#include "SDL_include.h"

#include "Vec2.h"

#define MOUSE_BUTTON_LEFT   SDL_BUTTON_LEFT
#define MOUSE_BUTTON_RIGHT  SDL_BUTTON_RIGHT
#define KEY_ESCAPE          SDLK_ESCAPE
#define KEY_MINUS           SDLK_MINUS
#define KEY_EQUAL           SDLK_EQUALS
#define KEY_W               SDLK_w
#define KEY_A               SDLK_a
#define KEY_S               SDLK_s
#define KEY_D               SDLK_d
#define KEY_Q               SDLK_q
#define KEY_E               SDLK_e
#define KEY_P               SDLK_p
#define KEY_SPACE           SDLK_SPACE
#define KEY_SHIFT_LEFT      SDLK_LSHIFT
#define KEY_CTRL_LEFT       SDLK_LCTRL
#define KEY_ARROW_UP        SDLK_UP
#define KEY_ARROW_LEFT      SDLK_LEFT
#define KEY_ARROW_DOWN      SDLK_DOWN
#define KEY_ARROW_RIGHT     SDLK_RIGHT

//Joypad
#define DEADZONE 8000
#define AXIS_LX SDL_CONTROLLER_AXIS_LEFTX
#define AXIS_LY SDL_CONTROLLER_AXIS_LEFTY
#define AXIS_RX SDL_CONTROLLER_AXIS_RIGHTX
#define AXIS_RY SDL_CONTROLLER_AXIS_RIGHTY
#define TRIGGER_L SDL_CONTROLLER_AXIS_TRIGGERLEFT
#define TRIGGER_R SDL_CONTROLLER_AXIS_TRIGGERRIGHT

#define JOY_A SDL_CONTROLLER_BUTTON_A
#define JOY_B SDL_CONTROLLER_BUTTON_B
#define JOY_X SDL_CONTROLLER_BUTTON_X
#define JOY_Y SDL_CONTROLLER_BUTTON_Y
#define JOY_BACK SDL_CONTROLLER_BUTTON_BACK
#define JOY_GUIDE SDL_CONTROLLER_BUTTON_GUIDE
#define JOY_START SDL_CONTROLLER_BUTTON_START
#define JOY_L3 SDL_CONTROLLER_BUTTON_LEFTSTICK
#define JOY_R3 SDL_CONTROLLER_BUTTON_RIGHTSTICK
#define JOY_L1 SDL_CONTROLLER_BUTTON_LEFTSHOULDER
#define JOY_R1 SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
#define JOY_UP SDL_CONTROLLER_BUTTON_DPAD_UP
#define JOY_DOWN SDL_CONTROLLER_BUTTON_DPAD_DOWN
#define JOY_LEFT SDL_CONTROLLER_BUTTON_DPAD_LEFT
#define JOY_RIGHT SDL_CONTROLLER_BUTTON_DPAD_RIGHT


class InputManager {
    private:
        std::unordered_map<int,bool> keyState;
        std::unordered_map<int,int> keyUpdate;
        bool mouseState[6];
        int mouseUpdate[6];
        int mouseX, mouseY;
        int updateCounter;
        bool quitRequested;

        SDL_GameController* controller;
        
        bool controllerState[21];
        int controllerUpdate[21];
        int axis[6];

    
    public:
        static InputManager& GetInstance();
        InputManager();
        ~InputManager();
        void Update();
        bool KeyPress(int key);
        bool KeyRelease(int key);
        bool IsKeyDown(int key);
        bool MousePress(int button);
        bool MouseRelease(int button);
        bool IsMouseDown(int button);
        Vec2 GetMousePosition();
        int GetMouseX();
        int GetMouseY();
        bool QuitRequested();
        int GetAxisMotion(int Index);
        
        bool ControllerPress(int key);
        bool ControllerRelease(int key);
        bool IsControllerDown(int key);
        // Vec2 GetLeftAxis()
};

#endif