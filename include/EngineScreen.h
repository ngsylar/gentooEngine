#ifndef OPENING_SCENE_STATE_H
#define OPENING_SCENE_STATE_H

#include "Timer.h"
#include "State.h"

#define OPENING_LABEL           "GameOpening"
#define OPENING_BLANK_SPRITE    "assets/ui/blank.k3y"
#define OPENING_ENGINE_SPRITE   "assets/ui/engine.k3y"
#define OPENING_DISPLAY_TIME    2.0f
#define OPENING_CONCEAL_TIME    0.25f
#define OPENING_LAYER           0

#define OPENING_GAME_SPRITES    {}

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