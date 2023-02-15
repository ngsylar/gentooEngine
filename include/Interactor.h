#ifndef INTERACTOR_H
#define INTERACTOR_H

#include "GentooEngine.h"


class Interactor : public Component {
    private:
        bool(*Condition)();
        void(*Result)();
        bool selfDestruct;
        ComponentType interactWith;

    public:
        Interactor(GameObject& associated,
            bool selfDestruct = false,
            bool notSolid = true,
            ComponentType interactWith = ComponentType::_Kid);
        ~Interactor();
        void SetCondition(bool(*Condition)());
        void SetResult(void(*Result)());

        bool Is(ComponentType type);
        void NotifyCollision(GameObject& other);
} ;

#endif//INTERACTOR_H