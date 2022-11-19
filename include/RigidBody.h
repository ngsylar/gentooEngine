#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "Component.h"

class RigidBody: public Component {
    private:
        int pixelScale;
        float gravity, acceleration;
        Vec2 force, velocity;
        Vec2 previousPosition, movementDirection;
    
        void HandleGravity(Vec2 position, float dt);

    public:
        bool grounded, pastedLeft, pastedRight;

        RigidBody(GameObject& associated);
        void Start();
        void Update(float dt);
        void AddForce(Vec2 force);
        void NotifyCollision(GameObject& other);
        bool Is(std::string type);
};

#endif