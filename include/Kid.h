#ifndef KID_H
#define KID_H

#include "EntityMachine.h"
#include "RigidBody.h"
#include "Collider.h"
#include "CameraBox.h"
#include "Timer.h"
#include "KidAttackMelee.h"
#include "FoxAttack.h"

class Kid: public EntityMachine {
    private:
        static GameObject* instance;
        RigidBody* rigidBody;
        Collider* collider;
        Timer jumpTimer, attackTimer, spellTimer, damageTimer, invincibilityTimer;
        bool isInvincible, isDead;
        int hp;

        // Attack Types
        KidAttackMelee* swordAttackOnGround;
        FoxAttack* foxAttack;

        // Automatic Signals
        bool attackPerforming, damagePerforming;
        bool runSpeedReset, attackImpulseCancel;

        // Automatic Activation Values
        float attackOriginX, fallOriginY;
        Vec2 damageOrigin, damageForce;
        float damageImpulse;
        float flickDirection;
        int lastDirectionX;

        // Automatic Factors
        float runSpeedIncrease, jumpSpeedDecrease;
        float flickFactor;

        // Collision Faces
        bool isGrounded, hitCeiling, hitWall;

        // Camera Assistants
        GameObject* cameraBox;
        Timer cameraGroundedTimer;

        bool NewStateRule(EntityState newState, int argsc, float argsv[]);
        void AttackStart();
        void AttackUpdate(float dt);
        void InvincibleUpdate(float dt);
        void Die();
        void ColliderReset();

        // Camera Assistants
        void* CameraEffects();

    public:
        Kid(GameObject& associated);
        ~Kid();
        static GameObject* GetInstance();
        void Awaken();
        void Start();
        void LateUpdate(float dt);
        void UpdateEntity(float dt);
        void NotifyCollision(GameObject& other);
        bool IsInvincible();
        bool Is(ComponentType type);
};

#endif