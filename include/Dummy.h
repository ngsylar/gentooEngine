#ifndef DUMMY_H
#define DUMMY_H

#include "EntityMachine.h"
#include "Timer.h"

class Dummy: public EntityMachine {
    private:
        int HP;
        int lastDirection;
        Timer jumpLimit;
        bool ceiling;
        float jumpDecrease;

    public:
        Dummy(GameObject& associated);
        // void Attack();
        // void Run();
        // void Walk();
        // void Idle();
        // void Jump();
        // void Fall();

        //inheritance
        void NotifyCollision(GameObject& other);
        void StartEntity();
        void UpdateEntity(float dt);

        // bool Is(ComponentType type);
        // void NotifyNoCollision(GameObject& other);
};

#endif //DUMMY_H