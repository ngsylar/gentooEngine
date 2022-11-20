#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <utility>

#include "Component.h"

class RigidBody: public Component {
    private:
        enum CollisionType {UP, DOWN, LEFT, RIGHT};

        int pixelScale;
        float gravity, acceleration;
        Vec2 force, velocity;
        Vec2 previousPosition, movementDirection;
        std::vector<std::pair<std::weak_ptr<GameObject>, CollisionType>> collidingObjects;
    
        void HandleGravity(Vec2 position, float dt);

    public:
        // enum BodyType {DYNAMIC, KINEMATIC, STATIC};

        // BodyType bodyType;
        bool collisionFace[4];

        RigidBody(GameObject& associated);
        void Start();
        void Update(float dt);
        void AddForce(Vec2 force);
        void Translate(Vec2 displacement);
        void NotifyCollision(GameObject& other);
        void NotifyNoCollision(GameObject& other);
        bool Is(std::string type);
};

#endif