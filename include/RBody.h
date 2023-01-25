#ifndef R_BODY_H
#define R_BODY_H

#include "Component.h"
#include "GentooEngine.h"

class RBody : public Component {
    private:
        float mass;
        float gravity;

        Vec2 force;
        Vec2 friction;
        Vec2 speed;
        Vec2 acceleration;

    public:
        RBody(GameObject& associated);
        ~RBody();
        void SetMass(float mass);
        void SetGravity(float gravity);
        void ApplyForce(Vec2 force);
        void ApplyForceOnX(float x);
        void ApplyForceOnY(float y);
        void ResetForce();

        void ApplyFriction(Vec2 friction);
        void ResetFriction();

        //inheritance
        bool Is(ComponentType type);
        bool Is(std::string type);
        void NotifyCollision(GameObject& other);
        void NotifyNoCollision(GameObject& other);
        void Render();
        void Start();
        void Update(float dt);
        void LateUpdate(float dt);

};



#endif// R_BODY_H
