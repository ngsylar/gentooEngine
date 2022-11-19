#ifndef ENGINE_SCREEN_STATE_H
#define ENGINE_SCREEN_STATE_H

#include "Timer.h"
#include "State.h"

#define ENGINE_SCREEN_LABEL           "EngineScreen"
#define ENGINE_SCREEN_BLANK_SPRITE    "assets/ui/blank.k3y"
#define ENGINE_SCREEN_ENGINE_SPRITE   "assets/ui/engine.k3y"
#define ENGINE_SCREEN_DISPLAY_TIME    2.0f
#define ENGINE_SCREEN_CONCEAL_TIME    0.25f
#define ENGINE_SCREEN_LAYER           0

#define ENGINE_SCREEN_GAME_SPRITES    {}

class EngineScreen: public State {
    private:
        Timer screenTimer;
        int currentScreen;
        bool exchanger;
    
    public:
        EngineScreen();
        void LoadAssets();
        void Start();
        void Update(float dt);
        void RenderBase();
        std::weak_ptr<GameObject> AddObject(GameObject* object);
};

#endif