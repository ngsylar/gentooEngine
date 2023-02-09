#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <unordered_map>

#define INCLUDE_SDL
#include "SDL_include.h"

#include "Vec2.h"

// Keyboard
#define MOUSE_BUTTON_LEFT   SDL_BUTTON_LEFT
#define MOUSE_BUTTON_MIDDLE SDL_BUTTON_MIDDLE
#define MOUSE_BUTTON_RIGHT  SDL_BUTTON_RIGHT
#define KEY_ESCAPE          SDLK_ESCAPE
#define KEY_QUOTE           SDLK_QUOTEDBL
#define KEY_EXCLAIM         SDLK_EXCLAIM
#define KEY_TAB             SDLK_TAB
#define KEY_1               SDLK_1
#define KEY_2               SDLK_2
#define KEY_3               SDLK_3
#define KEY_4               SDLK_4
#define KEY_5               SDLK_5
#define KEY_6               SDLK_6
#define KEY_7               SDLK_7
#define KEY_8               SDLK_8
#define KEY_9               SDLK_9
#define KEY_MINUS           SDLK_MINUS
#define KEY_EQUAL           SDLK_EQUALS
#define KEY_A               SDLK_a
#define KEY_B               SDLK_b
#define KEY_C               SDLK_c
#define KEY_D               SDLK_d
#define KEY_E               SDLK_e
#define KEY_F               SDLK_f
#define KEY_G               SDLK_g
#define KEY_H               SDLK_h
#define KEY_I               SDLK_i
#define KEY_J               SDLK_j
#define KEY_K               SDLK_k
#define KEY_L               SDLK_l
#define KEY_M               SDLK_m
#define KEY_N               SDLK_n
#define KEY_O               SDLK_o
#define KEY_P               SDLK_p
#define KEY_Q               SDLK_q
#define KEY_R               SDLK_r
#define KEY_S               SDLK_s
#define KEY_T               SDLK_t
#define KEY_U               SDLK_u
#define KEY_W               SDLK_w
#define KEY_X               SDLK_x
#define KEY_Y               SDLK_y
#define KEY_Z               SDLK_z
#define KEY_COMMA           SDLK_COMMA
#define KEY_PERIOD          SDLK_PERIOD
#define KEY_SHIFT_LEFT      SDLK_LSHIFT
#define KEY_CTRL_LEFT       SDLK_LCTRL
#define KEY_SPACE           SDLK_SPACE
#define KEY_SHIFT_RIGTH     SDLK_RSHIFT
#define KEY_CTRL_RIGTH      SDLK_RCTRL
#define KEY_ENTER           SDLK_RETURN
#define KEY_ARROW_UP        SDLK_UP
#define KEY_ARROW_LEFT      SDLK_LEFT
#define KEY_ARROW_DOWN      SDLK_DOWN
#define KEY_ARROW_RIGHT     SDLK_RIGHT

// Joypad
#define STICK_DEADZONE      8000
#define STICK_AXIS_LX       SDL_CONTROLLER_AXIS_LEFTX
#define STICK_AXIS_LY       SDL_CONTROLLER_AXIS_LEFTY
#define STICK_AXIS_RX       SDL_CONTROLLER_AXIS_RIGHTX
#define STICK_AXIS_RY       SDL_CONTROLLER_AXIS_RIGHTY
#define JOY_TRIGGER_L       SDL_CONTROLLER_AXIS_TRIGGERLEFT
#define JOY_TRIGGER_R       SDL_CONTROLLER_AXIS_TRIGGERRIGHT
#define JOY_A               SDL_CONTROLLER_BUTTON_A
#define JOY_B               SDL_CONTROLLER_BUTTON_B
#define JOY_X               SDL_CONTROLLER_BUTTON_X
#define JOY_Y               SDL_CONTROLLER_BUTTON_Y
#define JOY_BACK            SDL_CONTROLLER_BUTTON_BACK
#define JOY_GUIDE           SDL_CONTROLLER_BUTTON_GUIDE
#define JOY_START           SDL_CONTROLLER_BUTTON_START
#define JOY_L3              SDL_CONTROLLER_BUTTON_LEFTSTICK
#define JOY_R3              SDL_CONTROLLER_BUTTON_RIGHTSTICK
#define JOY_L1              SDL_CONTROLLER_BUTTON_LEFTSHOULDER
#define JOY_R1              SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
#define JOY_UP              SDL_CONTROLLER_BUTTON_DPAD_UP
#define JOY_DOWN            SDL_CONTROLLER_BUTTON_DPAD_DOWN
#define JOY_LEFT            SDL_CONTROLLER_BUTTON_DPAD_LEFT
#define JOY_RIGHT           SDL_CONTROLLER_BUTTON_DPAD_RIGHT

class InputManager {
    private:
        static InputManager* inputManager;
        std::unordered_map<int,bool> keyState;
        std::unordered_map<int,int> keyUpdate;
        bool mouseState[6];
        int mouseUpdate[6];
        int mouseX, mouseY;
        int updateCounter;

        SDL_GameController* controller;
        bool controllerState[21];
        int controllerUpdate[21];
        int axis[6];

        bool skipFrames, quitRequested;
    
    public:
        static InputManager& GetInstance();
        InputManager();
        ~InputManager();
        void Update();
        void WindowEvent(SDL_Event& event);
        bool KeyPress(int key);
        bool KeyRelease(int key);
        bool IsKeyDown(int key);
        bool MousePress(int button);
        bool MouseRelease(int button);
        bool IsMouseDown(int button);
        Vec2 GetMousePosition();
        int GetMouseX();
        int GetMouseY();

        bool ControllerPress(int key);
        bool ControllerRelease(int key);
        bool IsControllerDown(int key);
        int GetAxisMotion(int Index);
        // Vec2 GetLeftAxis()

        void DontSkipFrames();
        bool SkippingFrames();
        bool QuitRequested();
};

#endif