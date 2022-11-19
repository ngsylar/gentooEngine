#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "Component.h"

class RigidBody: public Component {
    private:
        int pixelScale;
        float gravity;
        float acceleration, velocity;
        Vec2 previousPosition, movementDirection;
    
    public:
        bool grounded, pastedLeft, pastedRight;

        RigidBody(GameObject& associated);
        void Start();
        void Update(float dt);
        void HandleGravity(Vec2 position, float dt);
        void NotifyCollision(GameObject& other);
        bool Is(std::string type);
};

#endif