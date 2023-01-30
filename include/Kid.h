#ifndef KID_H
#define KID_H

#include "EntityMachine.h"
#include "RigidBody.h"
#include "Collider.h"
#include "CameraBox.h"
#include "Timer.h"
#include "KidAttackMelee.h"

class Kid: public EntityMachine {
    private:
        RigidBody* rigidBody;
        Collider* collider;
        Timer jumpTimer, invincibilityTimer;
        bool isInvincible;
        int hp;

        KidAttackMelee* attackMelee;

        // Automatic Factors
        Vec2 damageOrigin;
        float speedRunIncrease, speedJumpDecrease;
        int lastDirectionX;

        // Collision Faces
        bool isGrounded, hitCeiling, hitWall;

        // Camera Assistants
        GameObject* cameraBox;
        Timer cameraGroundedTimer, cameraShakeTimer;
        std::queue<Vec2> cameraShakeQueue;
        Vec2 cameraShakeReset;

        bool NewStateRule(EntityState newState);

        // Camera Assistants
        void* CameraEffects();

    public:
        Kid(GameObject& associated);
        void Awaken();
        void Start();
        void LateUpdate(float dt);
        void UpdateEntity(float dt);
        void NotifyCollision(GameObject& other);
        bool Is(ComponentType type);

        // Camera Assistants
        void CameraStartShake();
};

#endif