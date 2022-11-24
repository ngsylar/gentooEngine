#ifndef COMPONENT_H
#define COMPONENT_H

#include "GameObject.h"

class Component {
    protected:
        GameObject& associated;

    public:
        Component(GameObject& associated);
        virtual ~Component();
        virtual void Start();
        virtual void Update(float dt);
        virtual void LateUpdate(float dt);
        virtual void Render();
        virtual void NotifyCollision(GameObject& other);
        virtual void NotifyNoCollision(GameObject& other);
        virtual bool Is(std::string type);
};

#endif