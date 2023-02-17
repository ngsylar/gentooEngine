#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "GentooEngine.h"
#include "ZoneTransition.h"
#include "State.h"

class Menu : public State {
    private:
        GameObject* startGO;
        GameObject* creditsGO;
        GameObject* exitGO;

        Sprite* start;
        Sprite* credits;
        Sprite* exit;

        Rect rStart;
        Rect rCredits;
        Rect rExit;

        Timer changeState;
        bool fading;

        int highlighted;

    public:
        Menu();
        void LoadAssets();
        void Update(float dt);
};

class Credits : public State {
    private:
        Timer changeState;
        bool fading;

    public:
        Credits();
        void LoadAssets();
        void Update(float dt);
};

//Surface levels
class H1 : public State {
    private:

    public:
        H1();
        void LoadAssets();
        void Update(float dt);
};

class S1 : public State {
    private:

    public:
        S1();
        void LoadAssets();
        void Update(float dt);
};

class S2 : public State {
    private:

    public:
        S2();
        void LoadAssets();
        void Update(float dt);
};

class S3 : public State {
    private:

    public:
        S3();
        void LoadAssets();
        void Update(float dt);
};

class S4 : public State {
    private:

    public:
        S4();
        void LoadAssets();
        void Update(float dt);
};


//Underground Levels

class U1 : public State {
    private:

    public:
        U1();
        void LoadAssets();
        void Update(float dt);
};

class U2 : public State {
    private:

    public:
        U2();
        void LoadAssets();
        void Update(float dt);
};

class U3 : public State {
    private:

    public:
        U3();
        void LoadAssets();
        void Update(float dt);
};

class U4 : public State {
    private:

    public:
        U4();
        void LoadAssets();
        void Update(float dt);
};

class U5 : public State {
    private:

    public:
        U5();
        void LoadAssets();
        void Update(float dt);
};

class U6 : public State {
    private:

    public:
        U6();
        void LoadAssets();
        void Update(float dt);
};

class U7 : public State {
    private:

    public:
        U7();
        void LoadAssets();
        void Update(float dt);
};

class U8 : public State {
    private:

    public:
        U8();
        void LoadAssets();
        void Update(float dt);
};

class U9 : public State {
    private:

    public:
        U9();
        void LoadAssets();
        void Update(float dt);
};

class U10 : public State {
    private:

    public:
        U10();
        void LoadAssets();
        void Update(float dt);
};

class U11 : public State {
    private:

    public:
        U11();
        void LoadAssets();
        void Update(float dt);
};

class U12 : public State {
    private:

    public:
        U12();
        void LoadAssets();
        void Update(float dt);
};

class U13 : public State {
    private:

    public:
        U13();
        void LoadAssets();
        void Update(float dt);
};

class U14 : public State {
    private:

    public:
        U14();
        void LoadAssets();
        void Update(float dt);
};

class U15 : public State {
    private:

    public:
        U15();
        void LoadAssets();
        void Update(float dt);
};

class U16 : public State {
    private:

    public:
        U16();
        void LoadAssets();
        void Update(float dt);
};

#endif// GAMESCENE_H