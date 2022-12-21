#ifndef GAME_H
#define GAME_H

#include <memory>
#include <stack>

#define INCLUDE_SDL
#include "SDL_include.h"

#include "State.h"

#define WINDOW_TITLE_DEF    "Gentoo Engine"
#define WINDOW_SIZE_DEF     1024, 576
#define WINDOW_FLAGS        0
#define MIXER_CHANNELS      32
#define MIXER_CHUNK_SIZE    1024
#define RENDER_AUTO_DRIVE   -1
#define GAME_DELAY          33
#define GAME_SUCCESS        0
#define GAME_FAILURE        0

class Game {
    private:
        static Game* instance;
        SDL_Window* window;
        SDL_Renderer* renderer;
        std::stack<std::unique_ptr<State>> stateStack;
        State* storedState;
        std::string title;
        int width, height;
        Vec2 resolution;
        int frameStart;
        float dt;

        Game(
            std::string title, int width, int height,
            int logicalWidth=0, int logicalHeight=0);
        void CalculateDeltaTime();

    public:
        ~Game();
        float GetDeltaTime();
        static Game& GetInstance(
            std::string title, int width, int height,
            int logicalWidth=0, int logicalHeight=0);
        static Game& GetInstance();
        SDL_Renderer* GetRenderer();
        Vec2 GetResolution();
        void AddState(State* state);
        State& GetCurrentState();
        void Run();
        static void DeleteInstance();
};

#endif