#ifndef ENEMY_RUNNER_H
#define ENEMY_RUNNER_H

#include "EntityMachine.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Sprite.h"
#include "Timer.h"

class EnemyRunner: public EntityMachine {
    private:
        RigidBody* rigidBody;
        Collider* collider;
        int movementDirection;
        Vec2 currentRoute;
        Rect perceptionRange;   // note: x front, y above, w back, h below
        int hp;

        // Behaviour
        Timer toggleTimer, attackTimer, recoverTimer;

        // Automatic Factors
        Vec2 attackTarget;
        Timer edgeTimer;
        float damageOriginX, damageImpulse;
        int damageDirectionX, damageTaken;

        // Collision Faces
        bool isGrounded, hitWall;

        bool NewStateRule(EntityState newState, int argsc, float argsv[]);

    public:
        EnemyRunner(GameObject& associated);
        void Awaken();
        void Start();
        void LateUpdate(float dt);
        void UpdateEntity(float dt);
        void RenderEntity();
        void NotifyCollision(GameObject& other);
};

#endif