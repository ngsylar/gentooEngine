#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <utility>
#include <queue>

#include "Component.h"

class RigidBody: public Component {
    public:
        // enum BodyType {DYNAMIC, KINEMATIC, STATIC};
        enum Axes {HORIZONTAL, VERTICAL, ALL};
        enum ColliderFace {UP, DOWN, LEFT, RIGHT};

        float gravityValue;
        bool gravityEnabled;
        bool collidingFaces[4];

        RigidBody(GameObject& associated);
        void Start();
        void Update(float dt);
        bool IsGrounded();
        Vec2 GetVelocity();
        void Translate(Vec2 displacement);
        void AddForce(Vec2 force);
        void CancelForces(Axes axis=ALL);
        void NotifyCollision(GameObject& other);
        void NotifyNoCollision(GameObject& other);
        bool Is(std::string type);

    private:
        Vec2 velocity;
        Vec2 movementDirection;
        std::queue<Vec2> previousPosition;
        std::vector<std::pair<std::weak_ptr<GameObject>, ColliderFace>> collidingOthers;
    
        void CheckDeletedColliders();
        void HandleGravity();
};

#endif