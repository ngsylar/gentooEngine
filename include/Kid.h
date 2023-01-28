#ifndef KID_H
#define KID_H

#include "EntityMachine.h"
#include "RBody.h"
#include "Collider.h"
#include "CameraBox.h"
#include "Timer.h"

class Kid: public EntityMachine {
    private:
        RBody* rigidBody;
        Collider* collider;
        Timer jumpTimer;
        int hp;

        // Automatic Factors
        Vec2 damageOrigin;
        float speedRunFactor, speedJumpFactor;
        int lastDirectionX;

        // Collision Faces
        bool collidingUp, collidingDown;

        // Camera Assistants
        GameObject* cameraBox;
        Timer cameraGroundedTimer, cameraShakeTimer;
        std::queue<Vec2> cameraShakeQueue;
        Vec2 cameraShakeReset;

    public:
        Kid(GameObject& associated);
        void Awaken();
        void Start();
        void LateUpdate(float dt);
        void UpdateEntity(float dt);
        void NotifyCollision(GameObject& other);
        bool Is(ComponentType type);

        // Entity Specific Methods
        void TakeDamage(Vec2 damageOrigin, float dt);

        // Camera Assistants
        void CameraStartShake();
        void* CameraEffects();
};

#endif