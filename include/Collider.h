#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

class Collider: public Component {
    public:
        Rect box;
        Vec2 offset;
        bool isTrigger;
        bool started;

        Collider(
            GameObject& associated,
            Vec2 offset=Vec2(0, 0),
            bool trigger=false
        );
        void SetBox(Vec2 offset, Vec2 boxSize);
        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        bool Is(GameObjID type);
};

#endif