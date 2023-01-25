#ifndef DUMMY_H
#define DUMMY_H

#include "EntityMachine.h"

class Dummy : public EntityMachine {
    private:
        int HP;

    public:
        Dummy(GameObject& associated);

        void Attack();
        //inheritance
        // bool Is(ComponentType type);
        // void NotifyCollision(GameObject& other);
        // void NotifyNoCollision(GameObject& other);

        void StartEntity();
        void UpdateEntity(float dt);
        

};

#endif //DUMMY_H