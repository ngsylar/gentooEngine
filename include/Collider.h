#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

class Collider: public Component {
    private:
        Vec2 scale, offset;

    public:
        Rect box;

        Collider(
            GameObject& associated,
            Vec2 scale=Vec2(1, 1),
            Vec2 offset=Vec2(0, 0)
        );
        void Update(float dt);
        void Render();
        void SetScale(Vec2 scale);
        void SetOffset(Vec2 offset);
        bool Is(std::string type);
};

#endif