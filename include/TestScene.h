#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "Timer.h"
#include "State.h"

class TestScene: public State {
    private:
        std::weak_ptr<GameObject> mini_platform, square;

    public:
        TestScene();
        void LoadAssets();
        void Start();
        void Update(float dt);
};

class TestScene2: public State {
    private:
        std::weak_ptr<GameObject> mini_platform, square;

    public:
        TestScene2();
        void LoadAssets();
        void Start();
        void Update(float dt);
};

class TestScene3: public State {
    private:
        std::weak_ptr<GameObject> square;

    public:
        TestScene3();
        void LoadAssets();
        void Start();
        void Update(float dt);
};

class TestScene4: public State {
    private:
        std::weak_ptr<GameObject> square;

    public:
        TestScene4();
        void LoadAssets();
        void Update(float dt);
};

class TestScene5: public State {
    public:
        TestScene5();
        void LoadAssets();
        void Update(float dt);
};

#endif