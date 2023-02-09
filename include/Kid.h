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
        Timer jumpTimer, attackTimer, damageTimer, invincibilityTimer;
        bool isInvincible;
        int hp;

        // Attack Types
        KidAttackMelee* swordAttackOnGround;

        // Automatic Factors
        bool attackPerforming;
        float runSpeedIncrease, jumpSpeedDecrease;
        bool runSpeedReset, attackImpulseCancel;
        Vec2 damageOrigin, damageForce;
        float damageImpulse;
        int lastDirectionX;

        // Collision Faces
        bool isGrounded, hitCeiling, hitWall;

        // Camera Assistants
        GameObject* cameraBox;
        Timer cameraGroundedTimer;

        bool NewStateRule(EntityState newState, int argsc, float argsv[]);

        // Camera Assistants
        void* CameraEffects();

    public:
        Kid(GameObject& associated);
        void Awaken();
        void Start();
        void LateUpdate(float dt);
        void UpdateEntity(float dt);
        void AttackStart();
        void AttackUpdate(float dt);
        void NotifyCollision(GameObject& other);
        bool Is(ComponentType type);
};

#endif