#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <utility>

#include "Component.h"

class RigidBody: public Component {
    private:
        enum CollisionType {UP, DOWN, LEFT, RIGHT};

        Vec2 force, velocity;
        Vec2 previousPosition, movementDirection;
        std::vector<std::pair<std::weak_ptr<GameObject>, CollisionType>> collidingOthers;
    
        void HandleGravity();

    public:
        // enum BodyType {DYNAMIC, KINEMATIC, STATIC};

        // BodyType bodyType;
        bool collidingFaces[4];
        float gravity;

        RigidBody(GameObject& associated);
        void Start();
        void Update(float dt);
        bool IsGrounded();
        void Translate(Vec2 displacement);
        void AddForce(Vec2 force);
        void NotifyCollision(GameObject& other);
        void NotifyNoCollision(GameObject& other);
        bool Is(std::string type);
};

#endif