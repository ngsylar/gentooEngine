#ifndef COMPONENT_H
#define COMPONENT_H

#include "GameObject.h"
#include "GlobalDefinitions.h"

class Component {
    protected:
        GameObject& associated;

    public:
        ComponentType type = ComponentType::_None;

        Component(GameObject& associated);
        virtual ~Component();
        virtual void Start();
        virtual void Update(float dt);
        virtual void LateUpdate(float dt);
        virtual void Render();
        virtual void NotifyCollision(GameObject& other);
        virtual void NotifyNoCollision(GameObject& other);
        virtual bool Is(std::string type);
        virtual bool Is(ComponentType type);
};

#endif