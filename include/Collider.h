#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

class Collider: public Component {
    public:
        Rect box;
        Vec2 offset;
        bool isTrigger;
        bool setManually;

        Collider(GameObject& associated, bool trigger=false);
        void SetBox(Vec2 offset, Vec2 boxSize);
        void Awaken();
        void Update(float dt);
        void LateUpdate(float dt);
        void Render();
        bool Is(std::string type);
        bool Is(ComponentType type);
};

#endif