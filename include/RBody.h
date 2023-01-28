#ifndef R_BODY_H
#define R_BODY_H

#include "Component.h"

class RBody : public Component {
    private:
        float limitspeed;
        Vec2 friction;
        Vec2 speed;
        bool up, down, left, right;

    public:
        float gravity;

        RBody(GameObject& associated);
        ~RBody();

        float GetGravity();
        void SetGravity(float gravity);
        void ResetGravity();
        
        Vec2 GetSpeed();
        void SetSpeed(Vec2 force);
        void SetSpeedOnX(float x);
        void SetSpeedOnY(float y);
        void ResetSpeed();

        float GetLimitSpeed();
        void SetLimitSpeed(float limit);

        void ApplyFriction(Vec2 friction);
        void ResetFriction();

        bool ImpactUp();
        bool ImpactDown();
        bool ImpactLeft();
        bool ImpactRight();

        //inheritance
        bool Is(ComponentType type);
        bool Is(std::string type);
        void NotifyCollision(GameObject& other);
        void NotifyNoCollision(GameObject& other);
        void Render();
        void Start();
        void Update(float dt);
};

#endif// R_BODY_H