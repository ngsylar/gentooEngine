#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

#define COLLIDER_TRIGGER_TRUE   true
#define COLLIDER_TRIGGER_FALSE  false

class Collider: public Component {
    private:
        bool setManually;

    public:
        Rect box;
        Vec2 offset;
        bool isTrigger;

        Collider(GameObject& associated, bool trigger=false);
        void SetBox(Vec2 offset, Vec2 boxSize);
        void Reset();
        void Start();
        void Update(float dt);
        void LateUpdate(float dt);
        void Render();
        bool Is(std::string type);
        bool Is(ComponentType type);
};

#endif