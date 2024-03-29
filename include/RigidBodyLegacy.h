#ifndef RIGID_BODY_LEGACY_H
#define RIGID_BODY_LEGACY_H

#include <array>
#include <queue>
#include <utility>

#include "Component.h"

// editar: velocity esta sub-desenvolvida
class RigidBodyLegacy: public Component {
    public:
        // enum BodyType {DYNAMIC, KINEMATIC, STATIC};
        enum Axis {HORIZONTAL, VERTICAL, ALL};
        enum ColliderFace {UP, DOWN, LEFT, RIGHT};
        std::vector<std::string> noInteractionLabels;
        bool gravityEnabled;
        float gravityValue;
        Vec2 velocityMax;

        RigidBodyLegacy(GameObject& associated, float gravityValue=570.0f);
        ~RigidBodyLegacy();
        void Start();
        void Update(float dt);
        bool IsGrounded();
        Vec2 GetVelocity();
        void Translate(Vec2 displacement);
        void AddForce(Vec2 force);
        void CancelForces(Axis axis=ALL);
        void NotifyCollision(GameObject& other);
        void NotifyNoCollision(GameObject& other);
        bool IsColliding (ColliderFace face);
        bool Is(std::string type);
        bool Is(ComponentType type);

    private:
        Vec2 velocity;
        Vec2 movementDirection;
        std::queue<Vec2> previousPosition;
        std::vector<std::pair<std::weak_ptr<GameObject>, ColliderFace>> collidingOthers;
        std::array<bool, 4> collidingFaces;
    
        void HandleGravity(float dt);
        void CheckDeletedColliders();
};

#endif