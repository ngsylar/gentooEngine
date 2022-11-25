#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "Timer.h"
#include "State.h"

#define SCENE_TEST_LABEL "TestScene"
#define SCENE_TEST_BACKGROUND "assets/img/test_area.png"
#define SCENE_TEST_LAYER -1

#define SPRITE_RED_SQUARE "assets/img/red_square.png"
#define SPRITE_BLACK_SQUARE "assets/img/black_square.png"
#define SPRITE_RUN_RIGHT "assets/img/corrida1.png"
#define SPRITE_RUN_LEFT "assets/img/corrida2.png"

#define LAYER_RED_SQUARE 3
#define LAYER_RUN_RIGHT 1
#define LAYER_RUN_LEFT 0
#define LAYER_BLACK_SQUARE 2

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

// class TestScene3: public State {
//     private:
//         std::weak_ptr<GameObject> square;

//     public:
//         TestScene3();
//         void LoadAssets();
//         void Start();
//         void Update(float dt);
// };

#endif