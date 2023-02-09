#ifndef STATE_H
#define STATE_H

#include "GameObject.h"
#include "Timer.h"

class State {
    protected:
        bool debugMode, awake, popRequested, quitRequested;
        std::vector<std::shared_ptr<GameObject>> objectArray;
        std::vector<std::weak_ptr<GameObject>> renderingArray;
        bool scheduleSortingLayer;

        Music* stateMusic;
        GameObject FPSObj;
        Timer fpsLimiter;
    
    public:
        float stateDt;

        State();
        virtual ~State();
        virtual void AwakenBase();
        virtual void StartBase();
        virtual void UpdateBase(float dt);
        virtual void RenderBase();
        virtual void LoadAssets();
        virtual void Awaken();
        virtual void Start();
        virtual void Update(float dt);
        virtual void Render();
        virtual void Pause();
        virtual void Resume();
        virtual std::weak_ptr<GameObject> AddObject(GameObject* object);
        virtual std::weak_ptr<GameObject> GetObjectPtr(GameObject* object);
        virtual std::weak_ptr<GameObject> GetObjectPtr(std::string label);
        void ScheduleSort();
        void DetectCollisions();

        void FadeIn();
        Music* GetStateMusic();

        bool Debugging();
        void RequestPop();
        bool PopRequested();
        bool QuitRequested();
};

#endif